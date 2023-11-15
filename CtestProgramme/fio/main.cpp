
#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <string>
#include "FIO.h"

//Open csv file and save each value as string in vector textvalue

int main() {
    FIO fileIO = FIO();
    for (int j = 0; j < 5; j++) {
        std::string param = "test" + std::to_string(j);
        std::string filename = "test";
        filename.append(param);
        filename.append(".csv");
        fileIO.createAndOverwriteFile(filename);
        for (int i = 0; i < 5; i++) {
            fileIO.write2file(filename, i, i * 2.0);
        }
    }
}
