#ifndef PI_CONTROLLER_HPP
#define PI_CONTROLLER_HPP

// System Includes & definitions
#include <cmath>
#include <vector>
#include <math.h>
#define _USE_MATH_DEFINES

// DJI OSDK includes
#include <dji_vehicle.hpp>

// Helpers
#include <dji_linux_helpers.hpp>

class PIcontroller {
  public:
    float32_t PIvalue = 0.0;
    PIcontroller(float32_t,float32_t,float32_t); // Constructor
    void updatePIController(float32_t); // Serial port

  private:
    float32_t Kp = 0.0;
    float32_t Ki = 0.0;
    float32_t sampleTime = 0;
    float32_t sMinus1 = 0;
    float32_t sMinus2 = 0;
};
#endif // PI_CONTROLLER_HPP