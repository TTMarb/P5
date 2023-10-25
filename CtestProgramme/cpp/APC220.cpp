#include <iostream>
#include "APC220.h"

//Some functions only function on Linux
#ifdef __linux__
#include <string.h>
#include <termios.h>
#include <unistd.h>
#else
#include <string>
#endif

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

//A simple OStest, that prints the OS that the program is running on
void OStest() {
    std::string OS;
#ifdef __linux__
    OS = "Linux";
#else
    OS = "Windows";
#endif
    std::cout << OS << std::endl;
}

//Constructor
APC220::APC220() { std::cout << "Created class APC220" << std::endl; }

//Init function, that returns the serial port
int APC220::init() {
    std::cout << "Beginning init" << std::endl;
    //Most of this is only available for linux
#ifdef __linux__
    int serial_port = open("/dev/ttyTHS0", O_RDWR | O_NOCTTY);
    //Error detect for Serial port
    if (serial_port < 0) {
        std::cout << "Error " << errno << " from open: " << strerror(errno) << std::endl;
    }

    //Termios (Terminal IO) is used to setup the serial port
    struct termios tty;

    //Error detect for Termios
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    //Inspired by https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    /* Set Baud Rate - APC220s are setup @ 19200baud */
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
    //tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    //Test is attributes are set correctly
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    //Returns serial port
    std::cout << "Serial port: " << serial_port << ". Init done" << std::endl;
    return serial_port;
#else
    std::cout << "Doesn't run on a Windows machine" << std::endl;
    return -1;
#endif
}

bool APC220::write2radio(int serial_port, char msg[], unsigned int size) {
    char delim[] = "\r\n#";
#ifdef __linux__
    write(serial_port, msg, strlen(msg));
#else
    std::cout << "Windows: Haven't created write yet" << std::endl;
#endif
}

bool APC220::read2radio(int serial_port, char* outputarray, unsigned int outputLen) {
    char buffer[256];
    int readLen;
    bool msggood = true;
    for (int i; i < outputLen; i++) {
        //@TODO: Find noget bedre en '\0' at bruge
        outputarray[i] = '\0';
    }
    char delim[] = "#";
#ifdef __linux__
    int numberOfBytes = read(serial_port, &buffer, sizeof(buffer));
    int length = numberOfBytes - 1;
    if (length > outputLen) {
        msggood = false;
    }
    if (buffer[length - 1] != delim[0]) {
        msggood = false;
    }
    if (true == msggood) {
        std::cout << "Received: " << buffer << std::endl;
        strcpy(outputarray, buffer);
        return true;
    } else {
        return false;
    }
#else
    std::cout << "Windows: Haven't created write yet" << std::endl;
#endif
}