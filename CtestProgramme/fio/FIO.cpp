#include "FIO.h"

//Constructor
FIO::FIO() {
    if (mkdir(FIO::folderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        std::cout << "Folder created: " << folderName << std::endl;
    } else {
        std::cerr << "Error creating folder: " << folderName << std::endl;
    }
}

void FIO::int changeActiveFile(std::string filename) { FIO::activeFile = FIO::prependFolderToFilename(filename); }

int FIO::createFile() {
    std::ofstream outputFile(FIO::activeFile.c_str()); // create a new output file or overwrite an existing one
    if (outputFile.is_open()) {                        // check if the file was opened successfully
        outputFile << "DATA FROM TEST" << filename;    // write data to the file
        outputFile.close();                            // close the file when done
    } else {
        std::cerr << "Error opening file\n";
        return -1;
    }
    return 0;
}

int FIO::write2file(int time, float angle) {
    std::cout << "activeFile: " << FIO::activeFile << ", time: " << time << ", angle: " << angle << std::endl;
    std::ofstream outputFile(FIO::activeFile.c_str(), std::ios::app); // create a new output file
    if (outputFile.is_open()) {                                       // check if the file was opened successfully
        outputFile << "filename: " << filename << ", time: " << time << ", angle: " << angle << std::endl;
        outputFile.close(); // close the file when done
    } else {
        std::cerr << "Error opening file during append\n";
        return -1;
    }
    return 0;
}

std::string FIO::prependFolderToFilename() {
    filename.insert(0, "./" + FIO::folderName + "/"); // insert the folder name in front of the filename
    return filename;
}