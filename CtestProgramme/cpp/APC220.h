#include <iostream>

#ifdef _WIN64
#define vindues
#elif __linux__
#define linus
#endif

void OStest();

class APC220 {
  public:
    APC220(); // Constructor
  private:
};