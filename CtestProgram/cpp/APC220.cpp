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
APC220::APC220() {
    std::cout << "Beginning init" << std::endl;
    //Most of this is only available for linux
#ifdef __linux__
    int radio = open("/dev/ttyTHS0", O_RDWR | O_NOCTTY);
    //Error detect for Serial port
    if (radio < 0) {
        std::cout << "Error " << errno << " from open: " << strerror(errno) << std::endl;
    }

    //Termios (Terminal IO) is used to setup the serial port
    struct termios tty;

    //Error detect for Termios
    if (tcgetattr(radio, &tty) != 0) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    //Inspired by https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    /* Set Baud Rate - APC220s are setup @ 19200baud */
    cfsetspeed(&tty, B9600);
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

    //Test is attributes are set correctly
    if (tcsetattr(radio, TCSANOW, &tty) != 0) {
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    //Returns serial port
    std::cout << "Serial port: " << radio << ". Init done" << std::endl;
#else
    std::cout << "Doesn't run on a Windows machine" << std::endl;
    radio = -1;
#endif
}


bool APC220::write2radio(char msg[], unsigned int size) {
    char delim[] = "\r\n#";
    strcat(msg, delim);
#ifdef __linux__
    write(radio, msg, strlen(msg));
#else
    std::cout << "Windows: Haven't created write yet" << std::endl;
#endif
}

bool APC220::read2radio(char* outputarray, unsigned int outputLen) {
    //Creates a buffer to store the message
    char buffer[256];
    int readLen;
    bool msggood = true;
    //Clears the outputarray from previous messages
    for (int i; i < outputLen; i++) {
        outputarray[i] = '\0';
    }
    char delim[] = "#";
#ifdef __linux__
    int numberOfBytes = read(radio, &buffer, sizeof(buffer));
    //std::cout << "Received following junk: " << buffer;
    int length = numberOfBytes - 1;
    //Checks if the message length is shorter than output array
    if (length > outputLen) {
        msggood = false;
    }
    //Makes sure it is the right delimiter
    if (buffer[length - 1] != delim[0]) {
        msggood = false;
    }
    if (true == msggood) {
        std::cout << "Received: " << buffer << std::endl;
        //Removes the delimiter from the end of the message
        strncpy(outputarray, buffer, length - 1);
        return true;
    } else {
        //Returns false if message is unacceptable
        return false;
    }
#else
    std::cout << "Windows: Haven't created write yet" << std::endl;
#endif
}