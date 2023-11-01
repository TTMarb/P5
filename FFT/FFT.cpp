
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Matlab_libs/mat.h"

using namespace std;

int get_file_size(string file_name){
    int file_size;
    ifstream data_file(file_name); //File location from location of code file
    if (data_file.is_open()) { //Checks if file was opened
        float a;
        while (data_file >> a) { //While there is data push the next value to a
            file_size++; //Increment the size each iteration to get the array size
        }
        data_file.close(); //Close the file.
    }
    else { //If file couldn't be opened print out error
        cout << "Unable to open file";
    }
    cout << file_size <<endl;
    return file_size;
}

void matread(const char *file, vector<double>& v)
{
    // open MAT-file
    MATFile *pmat = matOpen(file, "r");
    if (pmat == NULL) return;

    // extract the specified variable
    mxArray *arr = matGetVariable(pmat, "LocalDouble");
    if (arr != NULL && mxIsDouble(arr) && !mxIsEmpty(arr)) {
        // copy data
        mwSize num = mxGetNumberOfElements(arr);
        double *pr = mxGetPr(arr);
        if (pr != NULL) {
            v.reserve(num); //is faster than resize :-)
            v.assign(pr, pr+num);
        }
    }

    // cleanup
    mxDestroyArray(arr);
    matClose(pmat);
}

int main() {

    //Create array with size of the array in the file
    float data_array[get_file_size("Test_Signals/100Hz_4096_signal.txt")]; 
    ifstream data_file("Test_Signals/100Hz_4096_signal.txt");
    for(int i=0; i>(sizeof(data_array)/sizeof(float));i++){
        data_file >> data_array[i];
    }
    data_file.close();
    cout << data_array[0] << data_array[111] << data_array[2000] << data_array[1024] <<endl;
    return 0;
}
