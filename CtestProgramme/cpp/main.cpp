#include <iostream>
#include <stdio.h>

#include "APC220.h"

using namespace std;

int main() {
    char msg[] = "\n\rHello from APC220\n\r";
    char received[10];
    OStest();
    APC220 radio = APC220();
    int serial_port = radio.init();
    radio.write2radio(serial_port, msg);
    radio.read2radio(serial_port, &received[0], sizeof(received));
}
