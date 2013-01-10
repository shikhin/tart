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

#define USB_TTY_DEVICE "/dev/ttyUSB0"

/*
 * The entry point of the program.
 */
int main(int argc, char *argv[])
{
    // The termios structure, to be configured for serial interface.
    struct termios Serial;

    const char *Device = USB_TTY_DEVICE;

    // Open the device, read/write, not the controlling tty, and non-blocking I/O.
    int FD = open(Device, O_RDWR | O_NOCTTY | O_NDELAY);
    if((FD == -1) || !isatty(FD)) 
    {
        perror("Failed to open TTY port");
        return EXIT_FAILURE;
    }

    // Get the attributes.
    if(tcgetattr(FD, &Serial) == -1)
    {
        perror("Failed to get attributes of device");
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    // Write the attributes.
    tcsetattr(FD, TCSAFLUSH, &Serial);

    // Process.

    // Close the device.
    close(FD);

    return EXIT_SUCCESS;
}