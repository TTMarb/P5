#include <iostream>

void OStest();

class APC220 {
  public:
    APC220(); // Constructor
    int radio; // Serial port
    bool write2radio(char[], unsigned int);
    bool read2radio(char*, unsigned int);

  private:
};