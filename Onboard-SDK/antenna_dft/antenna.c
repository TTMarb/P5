#include "antenna.h"
#include <math.h>

//This function converts the latitude to meters
double calcMfromLat(double posLat) { return posLat * EARTH_RADIUS; }

//This function converts the longitude to meters
double calcMfromLon(double posLat, double posLon) { return posLon * cos(posLat) * EARTH_RADIUS; }

/// @brief Calculate the angle (in degrees) between two vectors
/// @param vector1
/// @param vector2
/// @return Returns the angle between the vectors
float getAngle(float vector1, float vector2) {
    float angleBetweenVectors = atan2(vector1, vector2);
    //converts from -pi to pi to 0 to 2pi
    if (angleBetweenVectors < 0) {
        angleBetweenVectors += 2 * M_PI;
    }
    //converts from radians to degrees
    angleBetweenVectors *= 180.0 / M_PI;
    return angleBetweenVectors;
}