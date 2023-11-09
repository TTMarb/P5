
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

void chk_file_name(string file_name) {
    ifstream data_file(file_name); //File location from location of code file
    if (data_file.is_open()) {     //Checks if file was opened
        cout << "File can be opened" << endl;
    } else { //If file couldn't be opened print out error
        cout << "Unable to open file" << endl;
    }
    data_file.close();
}

int main() {
    chk_file_name("Test_Signals/100Hz_4096_signal.mat");
    return 0;
}
