#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

class FIO {
  public:
    FIO(); // Constructor
    void init(string);
    bool write2file(int, int, float);
    void chk_file_name(string);

  private:
};