#include <fstream>
#include <iostream>

class FIO {
  public:
    FIO(); // Constructor
    int init(string);
    bool write2file(int, int, float);
    void chk_file_name(string);

  private:
};