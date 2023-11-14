#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

class FIO {
  public:
    FIO(); // Constructor
    int init(std::string);
    bool write2file(int, int, float);
    void chk_file_name(std::string);

  private:
};