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

APC220::APC220() { std::cout << "Created class APC220" << std::endl; }

int APC220::init() {
    std::cout << "Beginning init" << std::endl;
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

    std::cout << "Serial port: " << serial_port << std::endl;
    std::cout << "Init done" << std::endl;
    return serial_port;
#else
    std::cout << "Doesn't run on a Windows machine" << std::endl;
    return -1;
#endif
}

bool APC220::write2radio(int serial_port, char msg[]) {
    char delim[] = "##\n";
    std::cout << "Writing to serial port" << std::endl;
    std::cout << "\t Size of msg: " << strlen(msg) << std::endl;
    std::cout << "\t Msg content: " << msg << std::endl;
#ifdef __linux__
    write(serial_port, msg, strlen(msg));
    //write(serial_port, delim, strlen(delim));
#else
    std::cout << "Windows: Haven't created write yet" << std::endl;
#endif
}

bool APC220::read2radio(int serial_port) {
    char buffer[256];
    int len;
    while (1) {

        const char delim = '#';
#ifdef __linux__
        int nob = read(serial_port, &buffer, sizeof(buffer));
        std::cout << "NUMBER OF BYTES READ: " << nob << std::endl;
        std::cout << "Read from serial port" << std::endl;
        std::cout << "\t Strlen of msg: " << strlen(buffer) << std::endl;
        std::cout << "\t Size of msg: " << sizeof(buffer) << std::endl;
        std::cout << "\t Contents of msg: " << buffer << std::endl;
        std::cout << "\t content in place 4: " << buffer[4] << std::endl;
        std::cout << "\t Len: " << len << std::endl;
        std::cout << "\t content in delim place: " << buffer[len] << std::endl;

        if (buffer[len] == delim) {
            std::cout << "\t\tbuffer[len]: " << buffer[len - 2] << std::endl;
            std::cout << "\t\tDelim: " << delim << std::endl;
        }
    }
#else
        std::cout << "Windows: Haven't created write yet" << std::endl;
#endif
}