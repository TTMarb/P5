#include <iostream>

void OStest();

class APC220 {
  public:
    APC220(); // Constructor
    int init();
    bool write2radio(int, char[], unsigned int);
    bool read2radio(int, char*, unsigned int);

  private:
};