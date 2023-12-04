
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

//Open csv file and save each value as string in vector textvalue. Col determine column. Set to above 2 to read everything
void get_string_from_file_csv(string file_name, vector<string>* text_value, int col) {

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

        if (col == 1){
            getline(ss, value, ',');
            text_value->push_back(value);
        }
        else if (col == 2){
            getline(ss, value, ',');
            getline(ss, value, ',');
            text_value->push_back(value);
        }
        else{
            while (getline(ss, value, ',')) { // Read each value using getline with ',' as the delimiter
            text_value->push_back(value); // Add the value to the vector -> is the same as (*textvalue).pushback
            }
        }

    }
    data_file.close();
}

//Convert string vector to float vector
void vec_str_to_vec_float(vector<string>* str, vector<float>* doub) {
    for (const string& str : *str) { //Iterate through the whole string vector
        try {
            float value = stod(str);
            doub->push_back(value);
        } catch (const invalid_argument& e) { //Catch errors which can be thrown by the stod func
            cout << "Invalid argument: " << e.what() << endl;
        } catch (const out_of_range& e) {
            cout << "Out of range: " << e.what() << endl;
        }
    }
}

//Returns the dft value of the signal at the specified frequency (value is normalized)
complex<float> dft(vector<float>* sig, double freq, double fs) {
    complex<float> result;
    float N = sig->size();
    double norm_freq = freq * (N/fs); //Freq normalized from oscillations per second to oscillations per samplesize
    vector<complex<float>> twid; //Twiddle factor
    complex<float> twid_temp;
    complex<float> freq_const = complex<float>((-1i) * 2.0 * M_PI * norm_freq); //Constant for given freq therefore removed from for loop for less calculations

    for (float value = 0; value < N; value++) {
        twid_temp = exp((freq_const * value) / N); //Calc the current twiddle factor
        twid.push_back(twid_temp);                     //Save all twiddle factors
    }

    for (float j = 0; j < N; j++) { //Multiply each element of the signal with the twiddle factor and sum it up
        result += ((*sig)[j]) * twid[j];
    }
    result = result / N; //Normalizes the value
    return result;
}

//Split vector v into new_size sized pieces (removes remainder)
vector<vector<float>> split_vec(vector<float>* v, int new_size) {
    int n = v->size();           //Saves the size of the vector which v points to
    int N_amount = n / new_size; //Amount of new vectors created
    vector<vector<float>> split; //Vector to hold all new vectors created
    vector<float> temp;          //Temp vector to hold each new vector value temporarily

    for (int i = 0; i < N_amount; i++) {
        //Assign the value of v(0) - v(new_size) to temp and then v(new_size) - v(new_size*2) etc.
        temp.assign(v->begin() + (i * new_size), v->begin() + ((i + 1) * new_size));
        split.push_back(temp); //Save temp value in the complete vector
    }

    return split;
}

//Performs the DFT function repeatedly with sample_size as its dft size per dft
vector<complex<float>> stdft(vector<float>* sig, double freq, int dft_size, double fs) {
    float tot_N = sig->size();       // Total sample size of the whole signal
    int N_dft = tot_N / dft_size; //Number of dft
    complex<float> temp;
    vector<complex<float>> result;

    //Create matrix that holds sig split into sig/dft_size number of row/columns with dft_size number of row/column
    vector<vector<float>> vec_seg = split_vec(sig, dft_size); 

    for (int i = 0; i < N_dft; i++) {
        temp = dft(&vec_seg[i], freq, fs); //Perform dft on first vector of data and store it in temp
        result.push_back(temp); //Save temp in result and iterate 
    }

    return result;
}

int main() {
    vector<float> data; //Vector to hold values from input file
    vector<string> data_string; //Firstly the file is read as strings so first it most be stored as such
    string file_path_in; 
    int col;

    cout << "Enter file path of the csv file you would like to perform the DFT on.\n(Remember windows copies path with '\' but / is needed here!)\n:";
    getline(cin, file_path_in); //Get user input file path
    cout << "Which column would you like to perform the dft on\n1 or 2?" << endl;
    cin >> col; //Get user input which column to calculate upon

    get_string_from_file_csv(file_path_in, &data_string, col); //Open file from user input file path and column. Save as vector with strings
    vec_str_to_vec_float(&data_string, &data); //Convert string vector to float vector
    //Perform dft multiple dft on <data>, frequency to search for, dft size, sample frequency.
    vector<complex<float>> output = stdft(&data, 457000, 2048, 1000000); 

    cout << "DFT done!" << endl;
    std::cout << "Enter a name for the file (without extension): ";
    std::string fileName;
    std::cin >> fileName; //Get user input for what to call file 
    fileName += ".csv";

    ofstream output_file(fileName); 
    if (!output_file.is_open()) { //In case filepath is invalid throw error
        std::cerr << "Error opening the file for writing." << std::endl;
        return 1; // Return an error code
    }

    for (int i = 0; i < output.size(); i++){
        output_file << output[i] << "\n";
    }
    output_file.close();

    return 0;
}
