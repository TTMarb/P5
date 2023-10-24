#include <iostream>
#include <stdio.h>

#include "APC220.h"

using namespace std;

int main() {
    char msg[] = "Bonjour world";
    OStest();
    APC220 radio = APC220();
    int serial_port = radio.init();
    radio.write(serial_port, msg);
}
