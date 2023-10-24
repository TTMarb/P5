#include <iostream>
#include "APC220.h"

#include <string>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#endif

void OStest() {
    std::string OS;
#ifdef __linux__
    OS = "Linux";
#else
    OS = "Windows";
#endif
    std::cout << OS << std::endl;
}

APC220::APC220() {
    std::cout << "Get fricked" << std::endl;
#ifdef __linux__
    int serial_port = open("/dev/ttyTHS0", O_RDWR | O_NOCTTY);

    if (serial_port < 0) {
        std::cout << "Error " << errno << " from open: " << strerror(errno) << std::endl;
    }

    struct termios tty;

    if (tcgetattr(serial_port, &tty) != 0) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    //Inspireret af https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    /* Set Baud Rate */
    cfsetspeed(&tty, B19200);
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ECHO;   // Disable echo
    tty.c_lflag &= ~ECHOE;  // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo

    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    std::cout << "About to write 'Bonjour World'" << std::endl;
    unsigned char msg[] = {'B', 'o', 'n', 'j', 'o', 'u', 'r', ' ', 'w', 'o', 'r', 'l', 'd', '\r', '\n'};
    write(serial_port, msg, sizeof(msg));
    std::cout << "About to write 'Gutentag Welt'" << std::endl;
    unsigned char msg1[] = {'G', 'u', 't', 'e', 'n', 't', 'a', 'g', ' ', 'w', 'e', 'l', 't', '\r', '\n'};
    write(serial_port, msg1, sizeof(msg1));
    std::cout << "Done" << std::endl;
#endif
}