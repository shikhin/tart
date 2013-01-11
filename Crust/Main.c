/*
 * The client-side part of the RPi loader.
 *
 * Copyright (c) 2013, Shikhin Sethi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 *     * The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define USB_TTY_DEVICE "/dev/ttyUSB0"

#define FLAGS_VERSION (1 << 0)
#define FLAGS_USAGE   (1 << 1)
#define FLAGS_HELP    (1 << 2)

#define GOOD_LENGTH   (1024)

typedef struct 
{
    // Path of device, and kernel.
    char *Device;
    char *Kernel;

    // Some flags.
    int Flags;
} Config_t;

/*
 * Outputs the help message.
 */
static inline void OutputHelpMsg()
{
    printf("Crust, version %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);

    printf("\t -d <device>\t\t Specifies the device path\n");
    printf("\t -k <kernel>\t\t Specifies the path of the kernel\n");
    printf("\t -v\t\t\t Used to display the Crust version\n");
    printf("\t -u\t\t\t Used to display the program usage\n");
    printf("\t -h\t\t\t Used to display this message\n");
}

/*
 * Outputs the usage message.
 */
static inline void OutputUsageMsg()
{
    printf("\t Usage:\t\t\tCrust [-d <device>] [-k <kernel>] [-u] [-v] [-h]\n");
}

/*
 * Outputs the version message.
 */
static inline void OutputVersionMsg()
{
    printf("Crust, version %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
}

/*
 * Parses the command line config.
 *     int argc     -> number of arguments.
 *     char *argv[] -> array of arguments.
 * 
 * Returns:
 *     Config_t     -> the config.
 */
Config_t ParseConfig(int argc, char *argv[])
{
    // The config structure.
    Config_t Config;

    // Flags.
    Config.Flags = 0;

    // The device string.
    Config.Device = USB_TTY_DEVICE;

    // The kernel's path.
    Config.Kernel = "./Tart.kern";

    // Parse the args.
    for(int i = 1; i < argc; i++)
    {
        // If it's not a flag, continue.
        if(argv[i][0] != '-')
            continue;

        switch(argv[i][1])
        {
            // If version is demanded, set the version flag.
            case 'v':
                Config.Flags |= FLAGS_VERSION;
                break;

            // If usage is demanded, set the usage flag.
            case 'u':
                Config.Flags |= FLAGS_USAGE;
                break;

            // If help is demanded, set the help flag.
            case 'h':
                Config.Flags |= FLAGS_HELP;
                break;

            // Get the device string.
            case 'd':
                // If the device string is really even there.
                if((i + 1) < argc)
                {
                    // Get the device string.
                    Config.Device = argv[++i];
                }
                break;

            // Get the kernel's path.
            case 'k':
                // If the kernel string is really even there.
                if((i + 1) < argc)
                {
                    // Get the kernel's path.
                    Config.Kernel = argv[++i];
                }
                break;
        }
    }

    return Config;
}

/*
 * Opens a serial port connection with a device.
 *     char *Device -> the path of the device to open the connection with.
 *
 * Returns:
 *     int          -> file descriptor of the opened connection.
 */
static int OpenSerialConnection(const char *Device)
{
    // The termios structure, to be configured for serial interface.
    struct termios Serial;

    // Open the device, read/write, not the controlling tty, and non-blocking I/O.
    int Conn = open(Device, O_RDWR | O_NOCTTY | O_NDELAY);
    if((Conn == -1) || !isatty(Conn)) 
    {
        // Close if opened, but not a tty.
        if(Conn != -1)
            close(Conn);

        perror("Failed to open TTY port");
        exit(EXIT_FAILURE);
    }

    // Get the attributes.
    if(tcgetattr(Conn, &Serial) == -1)
    {
        // Close the connection.
        close(Conn);

        perror("Failed to get attributes of device");
        exit(EXIT_FAILURE);
    }

    // So, we poll.
    Serial.c_cc[VTIME] = 0;
    Serial.c_cc[VMIN] = 0;

    // 8N1 mode, no input/output/line processing masks.
    Serial.c_iflag = 0;
    Serial.c_oflag = 0;
    Serial.c_cflag = CS8 | CREAD | CLOCAL;
    Serial.c_lflag = 0;

    // Set the baud rate.
    if((cfsetispeed(&Serial, B115200) < 0) || 
       (cfsetospeed(&Serial, B115200) < 0))
    {
        // Close the connection.
        close(Conn);

        perror("Failed to set baud-rate");
        exit(EXIT_FAILURE);
    }

    // Write the attributes.
    tcsetattr(Conn, TCSAFLUSH, &Serial); 

    return Conn; 
}

/* 
 * Read bytes via the serial connection.
 *     int Conn        -> connection handle where to read to.
 *     uint8_t *Stream -> stream from where to store the bytes.
 *     int NBytes      -> number of bytes to read.
 *
 * Returns:
 *     int             -> 0 for success, -1 for failure.
 */
static inline int ReadBytes(int Conn, uint8_t *Stream, int NBytes)
{
    // Read all the bytes.
    while(NBytes)
    {
        // Read the bytes, get how many read.
        int BytesRead = read(Conn, Stream, NBytes);
        if(BytesRead < 0)
        {
            return -1;
        }

        // Decrement bytes left to read, increment stream pointer.
        NBytes -= BytesRead;
        Stream += BytesRead;
    }

    return 0;
}

/* 
 * Write bytes via the serial connection.
 *     int Conn              -> connection handle where to write to.
 *     const uint8_t *Stream -> stream from where to get the bytes.
 *     int NBytes            -> number of bytes to write.
 *
 * Returns:
 *     int                   -> 0 for success, -1 for failure.
 */
static inline int WriteBytes(int Conn, const uint8_t *Stream, int NBytes)
{
    // Write all the bytes.
    while(NBytes)
    {
        // Write the bytes, get how many written.
        int BytesWritten = write(Conn, Stream, NBytes);
        if(BytesWritten < 0)
        {
            return -1;
        }

        // Decrement bytes left to write, increment stream pointer.
        NBytes -= BytesWritten;
        Stream += BytesWritten;
    }

    return 0;
}

/*
 * Transfers a file via the serial connection.
 *     int Conn   -> connection where to transfer the file to.
 *     FILE *File -> the file to transfer.
 *
 * Returns:
 *     int        -> status of transfer.
 */ 
static int TransferFile(int Conn, FILE *File)
{
    // A quick buffer on the stack of GOOD_LENGTH.
    uint8_t Buffer[GOOD_LENGTH];
 
    // Bytes read this time, and bytes to write.
    int BytesRead;

    // Keep reading till we don't encounter any error.
    do
    {
        BytesRead = fread(Buffer, 1, GOOD_LENGTH, File);

        if(ferror(File))
        {
            close(Conn); fclose(File);

            perror("Failed trying to read input file");
            exit(EXIT_FAILURE);
        }

        // Write the bytes.
        if(WriteBytes(Conn, (uint8_t*)&Buffer, BytesRead) == -1)
        {
            close(Conn); fclose(File);

            perror("Failed trying to write to device");
            exit(EXIT_FAILURE);
        }
    } while(BytesRead > 0);

    return EXIT_SUCCESS;
}

/*
 * The entry point of the program.
 */
int main(int argc, char *argv[])
{
    // Parse the config.
    Config_t Config = ParseConfig(argc, argv);

    if(Config.Flags & FLAGS_HELP)
    {
        // Print help message, and exit successfully.
        OutputHelpMsg();
        return EXIT_SUCCESS;
    }

    if(Config.Flags & FLAGS_VERSION)
    {
        // Print version message, and exit successfully.
        OutputVersionMsg();
        return EXIT_SUCCESS;
    }

    if(Config.Flags & FLAGS_USAGE)
    {
        // Print usage message, and exit successfully.
        OutputUsageMsg();
        return EXIT_SUCCESS;
    }

    // Open the serial port connection.
    int Conn = OpenSerialConnection(Config.Device);

    // Process.
    // Open the kernel for read.
    FILE *Kernel = fopen(Config.Kernel, "rb");
    if(!Kernel)
    {
        close(Conn);

        perror("Failed to open the kernel");
        exit(EXIT_FAILURE);
    }

    // A simple buffer for the beginnning.
    uint8_t Byte[2];
    uint32_t Size;
    
    // Send the version, and kernel size.
    Byte[0] = VERSION_MAJOR; Byte[1] = VERSION_MINOR;
    
    fseek(Kernel, 0L, SEEK_END);
    Size = ftell(Kernel);
    fseek(Kernel, 0L, SEEK_SET);

    // Writing failed.
    if(WriteBytes(Conn, (uint8_t*)&Byte, 2) == -1 ||
       WriteBytes(Conn, (uint8_t*)&Size, 4) == -1)
    {
        close(Conn); fclose(Kernel);

        printf("Failed trying to write to device.\n");
        exit(EXIT_FAILURE);
    }

    // Get OK validation.
    if(ReadBytes(Conn, (uint8_t*)&Byte, 2) == -1 ||
       Byte[0] != 'O' ||
       Byte[1] != 'K')
    {
        close(Conn); fclose(Kernel);

        switch(Byte[0])
        {
            // Version error.
            case 'V':
                printf("Version mis-match with loader.\n");

            // Size error.
            case 'S':
                printf("Kernel size not acceptable by loader.\n");
        }

        exit(EXIT_FAILURE);
    }

    // Transfer the file.
    TransferFile(Conn, Kernel);

    char C;

    while(1)
    {
        if(ReadBytes(Conn, (uint8_t*)&C, 1) == -1)
        {
            printf("Error reading input from kernel.\n");
            exit(EXIT_FAILURE);
        }
        
        printf("%c", C); fflush(stdout);
    }

    // Close the connection.
    close(Conn);

    return EXIT_SUCCESS;
}