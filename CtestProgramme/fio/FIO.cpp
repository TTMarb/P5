#include "FIO.h"

//Constructor
FIO::FIO() { std::cout << "Created FIO" << std::endl; }

//Init function, that returns the serial port
int FIO::createAndOverwriteFile(std::string filename) {
    std::ofstream outputFile(filename.c_str()); // create a new output file or overwrite an existing one
    if (outputFile.is_open()) {                 // check if the file was opened successfully
        outputFile << "Hello, world!\n";        // write data to the file
        outputFile.close();                     // close the file when done
        std::cout << "Data was written to output.txt\n";
    } else {
        std::cerr << "Error opening file\n";
    }
    outputFile.open(filename.c_str()); // open the file in append mode
    outputFile << "BONK!\n";
    outputFile.append("More data\n"); // append data to the file
    outputFile.close();               // close the file when done

    return 0;
}

bool FIO::write2file(std::string filename, int time, float angle) {
    std::cout << "filename: " << filename << ", time: " << time << ", angle: " << angle << std::endl;
    return true;
}

//only used for trouble shooting simply to check if file name / location is correct
void FIO::chk_file_name(std::string file_name) {
    /*ifstream data_file(file_name); //File location from location of code file
    if (data_file.is_open()) {     //Checks if file was opened
        cout << "File can be opened" << endl;
    } else { //If file couldn't be opened print out error
        cout << "Unable to open file" << endl;
    }
    data_file.close();*/
}
