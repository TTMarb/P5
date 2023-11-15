#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

class FIO {
  public:
    FIO(); // Constructor
    std::ofstream createAndOverwriteFile(std::string);
    bool write2file(std::ofstream, int, float);
    void chk_file_name(std::string);

  private:
};