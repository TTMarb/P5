#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

class FIO {
  public:
    FIO(); // Constructor
    int createFile(std::string);
    int write2file(std::string, int, float);

  private:
};