#include <iostream>

#ifdef _WIN64
#define OS "Windows"
#elif __linux__
#define OS "Linux"
#endif

void OStest();

//#include <string>

//#include <stdio.h>
//#include <errno.h>
//#include <fcntl.h>

//#include <termios.h>
//#include <unistd.h>

class APC220 {
  public:
    APC220(); // Constructor
  private:
};