#include "FIO.h"

//Constructor
FIO::FIO() {
    std::string folderName = "outputfolder";
    try {
        if (!fs::exists(folderName)) {
            // Create the folder
            fs::create_directory(folderName);
            std::cout << "Folder created: " << folderName << std::endl;
        } else {
            std::cerr << "Folder already exists: " << folderName << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    return 0;
}

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