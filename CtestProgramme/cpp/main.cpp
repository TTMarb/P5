#include <iostream>
#include <stdio.h>

#include "APC220.h"
#include "calculations.h"

using namespace std;

int main() {
    std::cout << "Hello World!" << std::endl;
    int result = add(3, 6);
    std::cout << "Result: " << result << std::endl;
    output_message("Hello World!");
    OStest();
    APC220();
}
