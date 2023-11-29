#ifndef ANTENNA_H
#define ANTENNA_H

#include <math.h>

double calcMfromLat(double posLat);

double calcMfromLon(double posLat, double posLon);

float getAngle(float vector1, float vector2);

#endif // ANTENNA_H