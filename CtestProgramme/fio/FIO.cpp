#include "FIO.h"

//Constructor
FIO::FIO() { std::cout << "Created FIO" << std::endl; }

//Init function, that returns the serial port
void FIO::init(string filename) {
    std::ofstream ofs(filename, std::ofstream::out);
    if (of.is_open()) {
        of << "Some text here" << std::endl;
        of.flush();
        of.close();
        std::cout << "wrote the file successfully!" << std::endl;
    } else {
        std::cerr << "Failed to open file : " << SYSERROR() << std::endl;
        return -1;
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
