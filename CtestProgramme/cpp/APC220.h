#include <iostream>

#ifdef _WIN64
#define vindues
#elif __linux__
#define linus
#endif

void OStest();

#include <string>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef linus
#include <termios.h>
#include <unistd.h>
#endif

class APC220 {
  public:
    APC220(); // Constructor
  private:
};