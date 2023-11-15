#include "FIO.h"

//Constructor
FIO::FIO() { std::cout << "Created FIO" << std::endl; }

//Init function, that returns the serial port
int FIO::createFile(std::string filename) {
    std::ofstream outputFile(filename.c_str());     // create a new output file or overwrite an existing one
    if (outputFile.is_open()) {                     // check if the file was opened successfully
        outputFile << "DATA FROM TEST" << filename; // write data to the file
        outputFile.close();                         // close the file when done
    } else {
        std::cerr << "Error opening file\n";
        return -1;
    }
    return 0;
}

int FIO::write2file(std::string filename, int time, float angle) {
    std::cout << "filename: " << filename << ", time: " << time << ", angle: " << angle << std::endl;
    std::ofstream outputFile(filename.c_str(), std::ios::app); // create a new output file or overwrite an existing one
    if (outputFile.is_open()) {                                // check if the file was opened successfully
        outputFile << "filename: " << filename << ", time: " << time << ", angle: " << angle << std::endl;
        outputFile.close(); // close the file when done
    } else {
        std::cerr << "Error opening file during append\n";
        return -1;
    }
    return 0;
}