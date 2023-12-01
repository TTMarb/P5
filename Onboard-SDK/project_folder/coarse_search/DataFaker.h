#ifndef DATA_FAKER_HPP
#define DATA_FAKER_HPP

// System Includes & definitions
#include <cmath>
#include <vector>
#include <math.h>
#include "FIO.h"
#include "coarse_search.h"
#define _USE_MATH_DEFINES

// DJI OSDK includes
#include <dji_vehicle.hpp>

// Helpers
#include <dji_linux_helpers.hpp>

class DataFaker {
  public:
    DataFaker(); // Constructor
    void init(Vehicle*, int, int, int); // Constructor
    void Fake(Vehicle*, FIO, bool); // Serial port
    float32_t A1;
    float32_t A2;

  private:
    float32_t sampleTime;
    float32_t iX;
    float32_t iY;
    float32_t tX;
    float32_t tY;
    int searchRadius;
};


float64_t calcMfromLat(Telemetry::GlobalPosition);

float64_t calcMfromLon(Telemetry::GlobalPosition);


#endif // DATA_FAKER_HPP