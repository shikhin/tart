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

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define USB_TTY_DEVICE "/dev/ttyUSB0"

#define FLAGS_VERSION (1 << 0)
#define FLAGS_USAGE   (1 << 1)
#define FLAGS_HELP    (1 << 2)

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

    // The device string.
    Config.Device = USB_TTY_DEVICE;

    // The kernel's path.
    Config.Kernel = "./tart.kern";

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
 *                     -1 to indicate failure.
 */
static int OpenSerialConnection(const char *Device)
{
    // The termios structure, to be configured for serial interface.
    struct termios Serial;

    // Open the device, read/write, not the controlling tty, and non-blocking I/O.
    int Conn = open(Device, O_RDWR | O_NOCTTY | O_NDELAY);
    if((Conn == -1) || !isatty(Conn)) 
    {
        perror("Failed to open TTY port");
        return -1;
    }

    // Get the attributes.
    if(tcgetattr(Conn, &Serial) == -1)
    {
        perror("Failed to get attributes of device");
        return -1;
    }

    // So, we return after 0.1 seconds.
    Serial.c_cc[VTIME] = 1;
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
        perror("Failed to set baud-rate");
        return -1;
    }

    // Write the attributes.
    tcsetattr(Conn, TCSAFLUSH, &Serial); 

    return Conn; 
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
    if(Conn == -1)
    {
        return EXIT_FAILURE;
    }

    // Process.

    // Close the connection.
    close(Conn);

    return EXIT_SUCCESS;
}