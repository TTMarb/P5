#include "FIO.h"

//Constructor
FIO::FIO() { std::cout << "Created FIO" << std::endl; }

//Init function, that returns the serial port
int FIO::init(string filename) {
    std::ofstream outputFile("test200.txt"); // create a new output file or overwrite an existing one
    if (outputFile.is_open()) {              // check if the file was opened successfully
        outputFile << "Hello, world!\n";     // write data to the file
        outputFile.close();                  // close the file when done
        std::cout << "Data was written to output.txt\n";
    } else {
        std::cerr << "Error opening file\n";
    }

    return 0;
}

bool FIO::write2file(int serial_port, int time, float angle) {
    std::cout << "Serial port: " << serial_port << std::endl;
    return true;
}

//only used for trouble shooting simply to check if file name / location is correct
void FIO::chk_file_name(string file_name) {
    ifstream data_file(file_name); //File location from location of code file
    if (data_file.is_open()) {     //Checks if file was opened
        cout << "File can be opened" << endl;
    } else { //If file couldn't be opened print out error
        cout << "Unable to open file" << endl;
    }
    data_file.close();
}
