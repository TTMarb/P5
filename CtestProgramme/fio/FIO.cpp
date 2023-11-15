#include "FIO.h"

//Constructor
FIO::FIO() {
    if (mkdir(FIO::folderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        std::cout << "Folder created: " << folderName << std::endl;
    } else {
        std::cerr << "Error creating folder: " << folderName << std::endl;
    }
}

int FIO::createFile(std::string filename) {
    filename.insert(0, "./" + FIO::folderName + "/"); // insert the folder name in front of the filename
    std::ofstream outputFile(filename.c_str());       // create a new output file or overwrite an existing one
    if (outputFile.is_open()) {                       // check if the file was opened successfully
        outputFile << "DATA FROM TEST" << filename;   // write data to the file
        outputFile.close();                           // close the file when done
    } else {
        std::cerr << "Error opening file\n";
        return -1;
    }
    return 0;
}

int FIO::write2file(std::string filename, int time, float angle) {
    filename.insert(0, "./" + FIO::folderName + "/"); // insert the folder name in front of the filename
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