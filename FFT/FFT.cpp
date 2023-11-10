
#define _USE_MATH_DEFINES

#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

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

//Open csv file and save each value as string in vector textvalue
void get_string_from_file_csv(string file_name, vector<string>* text_value) {

    string line;                   // Create string to temp contain value of a line from file
    ifstream data_file(file_name); //Open file and save to data_file

    if (!data_file.is_open()) { // Check if the file is opened successfully
        cerr << "Error opening file!" << endl;
        return;
    }

    while (getline(data_file, line)) { // Save line from data_file to line and repeat for all lines in file

        stringstream ss(line); // Use a stringstream to parse each line
        string value;          // String to temp contain each value from file
        //vector<string> values;  // Vector to store individual values

        while (getline(ss, value, ',')) { // Read each value using getline with ',' as the delimiter
            text_value->push_back(value); // Add the value to the vector -> is the same as (*textvalue).pushback
        }
    }
    data_file.close();
}

//Convert string vector to double vector
void vec_str_to_vec_double(vector<string>* str, vector<double>* doub) {
    for (const string& str : *str) { //Iterate through the whole string vector
        try {
            double value = stod(str);
            doub->push_back(value);
        } catch (const invalid_argument& e) { //Catch errors which can be thrown by the stod func
            cout << "Invalid argument: " << e.what() << endl;
        } catch (const out_of_range& e) {
            cout << "Out of range: " << e.what() << endl;
        }
    }
}

//Returns the dft value of the signal at the specified frequency (value is normalized)
complex<double> dft(vector<double>* sig, double freq) {
    complex<double> result;
    double N = sig->size();

    vector<complex<double>> twid; //Twiddle factor
    complex<double> twid_temp;
    complex<double> freq_const = (-1i) * 2.0 * M_PI * freq; //Constant for given freq therefore removed from for loop for less calculations
    for (const double& value : *sig){
        twid_temp = exp((freq_const * value)/N); //Calc the current twiddle factor
        twid.push_back(twid_temp); //Save all twiddle factors 
    }
    
    for (const double& value : *sig){

    }


    return result;
}

int main() {
    vector<double> data;
    vector<string> data_string;
    get_string_from_file_csv("Test_Signals/100Hz_4096_signal.csv", &data_string);
    vec_str_to_vec_double(&data_string, &data);

    return 0;
}
