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

#ifndef DJIOSDK_MISSIONSAMPLE_HPP
#define DJIOSDK_MISSIONSAMPLE_HPP

// System Includes
#include <cmath>
#include <libgen.h>
#include <vector>

// DJI OSDK includes
#include <dji_vehicle.hpp>

// Helpers
#include <dji_linux_helpers.hpp>

bool runWaypointMission(DJI::OSDK::Vehicle* vehicle, int numWaypoints, int responseTimeout, float64_t latM,
                        float64_t lonM);

void setWaypointDefaults(DJI::OSDK::WayPointSettings* wp);
void setWaypointInitDefaults(DJI::OSDK::WayPointInitSettings* fdata);

std::vector<DJI::OSDK::WayPointSettings> createWaypoints(DJI::OSDK::Vehicle* vehicle, int numWaypoints, float64_t latM,
                                                         float64_t lonM, DJI::OSDK::float32_t start_alt);

std::vector<DJI::OSDK::WayPointSettings> generateWaypoints(DJI::OSDK::WayPointSettings* start_data, int num_wp,
                                                           float64_t latM, float64_t lonM);

void uploadWaypoints(DJI::OSDK::Vehicle* vehicle, std::vector<DJI::OSDK::WayPointSettings>& wp_list,
                     int responseTimeout);

bool stopMission(DJI::OSDK::Vehicle* vehicle, int responseTimeout, int delayBeforeStop);

void setBroadcastFrequency(Vehicle* vehicle);

const int DEFAULT_PACKAGE_INDEX = 0;

void startProcess(pid_t pid, char* path, char* param);

float32_t QtoDEG(Vehicle* vehicle);

float32_t getAngle(float32_t vector1, float32_t vector2);

float movingAvg(float* ptrArrNumbers, float* ptrSum, int pos, int len, float nextNum);

#endif // DJIOSDK_MISSIONSAMPLE_HPP