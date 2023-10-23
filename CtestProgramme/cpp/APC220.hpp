#include <iostream>
#include <string>

#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

class APC220 {
  public:
    APC220(); // Constructor
    void add(int i, int j);

  private:
    std::string name;
};