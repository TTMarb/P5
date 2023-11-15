#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

class FIO {
  public:
    FIO(); // Constructor
    int createAndOverwriteFile(std::string);
    bool write2file(std::string, int, float);
    void chk_file_name(std::string);

  private:
};