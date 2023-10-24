#include <iostream>

void OStest();

class APC220 {
  public:
    APC220(); // Constructor
    bool write2radio(int, char[]);
    int init();
    bool read2radio(int, char*[]);

  private:
};