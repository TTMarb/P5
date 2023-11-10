
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>

using namespace std;

//Open csv file and save each value as string in vector textvalue
void get_string_from_file_csv(string file_name, vector<string> *text_value){

    string line; // Create string to temp contain value of a line from file
    ifstream data_file(file_name); //Open file and save to data_file
    
    if (!data_file.is_open()) { // Check if the file is opened successfully
        cerr << "Error opening file!" << endl;
        return;
    }
    
    while (getline(data_file, line)) { // Save line from data_file to line and repeat for all lines in file

        stringstream ss(line); // Use a stringstream to parse each line
        string value;  // String to temp contain each value from file
        //vector<string> values;  // Vector to store individual values

        while (getline(ss, value, ',')) {// Read each value using getline with ',' as the delimiter
            text_value->push_back(value); // Add the value to the vector -> is the same as (*textvalue).pushback
        }
    }
    data_file.close();
}

//only used for trouble shooting simply to check if file name / location is correct
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
    vector<double> data;
    vector<string> data_string;
    get_string_from_file_csv("Test_Signals/100Hz_4096_signal.csv",&data_string);
    for (const auto& val : data_string) {
            std::cout << val << " ";
    }
    return 0;
}
