/*! @file mission_sample.hpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  GPS Missions API usage in a Linux environment.
 *  Shows example usage of the Waypoint Missions and Hotpoint Missions through
 * the
 *  Mission Manager API.
 *
 *  @Copyright (c) 2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef COARSE_SEARCH_HPP
#define COARSE_SEARCH_HPP

// System Includes & definitions
#include <cmath>
#include <libgen.h>
#include <math.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "DataFaker.h"
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include "FIO.h"
#include "PIcontroller.h"
#include "sock.h"

#define EARTH_RADIUS (double)6378137.0

// DJI OSDK includes
#include <dji_vehicle.hpp>

// Helpers
#include <dji_linux_helpers.hpp>

void controlVehicle(DJI::OSDK::Vehicle*, float*, float*, FIO*, PIcontroller*, PIcontroller*, PIcontroller*, int, int*);
void UAStakoff(Vehicle*, int);
void UASstop(Vehicle*, bool, int);

float calcH(DJI::OSDK::Vehicle*, float*, float*, float*);
float calcAlg(DJI::OSDK::Vehicle*, float*, float*, float*);
float calcVel(DJI::OSDK::Vehicle*, float*, float*, int*, float);

float32_t getAngle(float32_t, float32_t);
float32_t QtoDEG(Vehicle*);
float32_t getSize(float32_t, float32_t);

// Only required for testing
void setBroadcastFrequency(Vehicle* vehicle);

void addRandomLocation(float32_t*, float32_t*, int, int);
#endif // DJIOSDK_MISSIONSAMPLE_HPP