#include <iostream>
#include <stdio.h>
//#define TESTMODE

#include "APC220.h"

using namespace std;

int main() {
    char msg[] = "\n\rHello from APC220\n\r";
    char received[10] = {'\0'};
    const int sizeOfReceived = sizeof(received) / sizeof(received[0]);
    OStest();
    APC220 radio = APC220();
    int serial_port = radio.init();
    radio.write2radio(serial_port, msg, sizeof(msg) / sizeof(msg[0]));
    while (1) {
        bool result = radio.read2radio(serial_port, &received[0], sizeOfReceived);
        std::cout << "Received: " << received << std::endl;
        int backsize = sizeof(received) / sizeof(received[0]);
        std::cout << "Result: " << backsize << std::endl;

        if (result) {
            radio.write2radio(serial_port, received, 5);
        }
    }
}
