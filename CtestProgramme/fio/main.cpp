
#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <string>
#include "FIO.h"
using namespace std;

//Open csv file and save each value as string in vector textvalue

int main() {
    FIO fileIO = FIO();
    std::string filename = "test.txt";
    fileIO.init(filename);
}
