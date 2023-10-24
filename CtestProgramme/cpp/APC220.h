#include <iostream>

#ifdef _WIN64
#define windows
#elif __linux__
#define linux
#endif

void OStest();

#include <string>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef linux
#include <termios.h>
#include <unistd.h>
#endif

class APC220 {
  public:
    APC220(); // Constructor
  private:
};