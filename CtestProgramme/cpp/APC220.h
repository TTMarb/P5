#include <iostream>

void OStest();

class APC220 {
  public:
    APC220(); // Constructor
    int init();
    bool write2radio(int, char[]);
    bool read2radio(int, char*);

  private:
};