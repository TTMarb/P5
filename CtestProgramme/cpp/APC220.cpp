#include <iostream>
#include "APC220.h"

#ifdef __linux__
#include <string.h>
#else
#include <string>
#endif

#include <cstring>
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
    char msg[] = "Bonjour World\r\n";

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
    APC220::writetorad(msg, serial_port);

    std::cout << "Serial port: " << serial_port << std::endl;
    std::cout << "Init done" << std::endl;
#endif
    APC220::writetorad(msg, 0);
}

bool APC220::writetorad(char msg[], int serial_port) {
    std::cout << "Writing to serial port" << std::endl;
    std::cout << "size of msg: " << sizeof(msg) << std::endl;
    std::cout << "About to write" << msg << std::endl;

#ifdef __linux__
    write(serial_port, str, sizeof(str));
#else
    std::cout << "Windows: Haven't created write yet" << std::endl;
#endif
}