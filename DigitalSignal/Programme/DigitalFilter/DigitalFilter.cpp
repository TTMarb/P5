#include <iostream>
#include <vector>
#include <string>

using namespace std;


void printer(int msg){

    std::cout << msg;
    
}

int input(){
    //cs low        pin 1
    //read clock falling edge (interrupt)      pin 4
    //discard first zero    data in     pin 8
    //store next 12 bits
    //convert into int
    //discard last two 0
    //pull cs high
}

void setup(){

}

void main(){
    printer(2);
    
}


