#include "FIO.h"

//Constructor
FIO::FIO() {
    //Appends string name to file buffer
    FIO::folderName.append(getTimeStamp());
    //Creates directory with name from buffer
    if (mkdir(FIO::folderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        std::cout << "Folder created: " << folderName << std::endl;
    } else {
        std::cerr << "Error creating folder: " << folderName << std::endl;
    }
}

void FIO::changeActiveFile(std::string filename) { activeFile = prependFolderToFilename(filename); }


int FIO::createFile() {
    std::ofstream outputFile(activeFile.c_str());     // create a new output file or overwrite an existing one
    if (outputFile.is_open()) {                            // check if the file was opened successfully
        outputFile << "Timestamp: "<<getTimeStamp() << "\n"; // write data to the file
        outputFile.close();                               // close the file when done
    } else {
        std::cerr << "Error opening file\n";
        return -1;
    }
    return 0;
}

int FIO::write2file(std::string data) {
    std::ofstream outputFile(activeFile.c_str(), std::ios::app); // create a new output file
    if (outputFile.is_open()) {                                  // check if the file was opened successfully
        outputFile << data << std::endl;
        outputFile.close(); // close the file when done
    } else {
        std::cerr << "Error opening file \"" <<activeFile << "\" during append\n";
        return -1;
    }
    return 0;
}

std::string FIO::prependFolderToFilename(std::string filename) {
    filename.insert(0, "./" + FIO::folderName + "/"); // insert the folder name in front of the filename
    return filename;
}

std::string FIO::getTimeStamp(){
    //Gets current time
    auto now = std::chrono::system_clock::now();
    // Convert to std::time_t
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    // Convert to std::tm (local time)
    std::tm localTime = *std::localtime(&currentTime);
    // Format the string "yymmddhhmmss"
    char buffer[30]; // Buffer to store the formatted string
    std::strftime(buffer, sizeof(buffer), "Y%Ym%md%d_h%Hm%Ms%S", &localTime); //Formats string from time
    return std::string(buffer);
}