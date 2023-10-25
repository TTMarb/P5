#include <iostream>
#include <stdio.h>
//#define TESTMODE

#include "APC220.h"

using namespace std;

int main() {
    char msg[] = "\n\rHello from APC220\n\r";
    char received[10];
    OStest();
    APC220 radio = APC220();
    int serial_port = radio.init();
    radio.write2radio(serial_port, msg);
    int size_received = sizeof(received) / sizeof(received[0]);
    while (1) {
        bool result = radio.read2radio(serial_port, &received[0], size_received);
        if (result) {
            radio.write2radio(serial_port, received);
        }
    }
}
