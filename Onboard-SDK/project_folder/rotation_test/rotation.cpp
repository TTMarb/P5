/*! @file mission_sample.cpp
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
 *
 */

#include "rotation.hpp"
#include <dji_broadcast.hpp>
#include <dji_telemetry.hpp>
#include <fstream>
#include <iostream>
#define _USE_MATH_DEFINES

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void getRotation(Vehicle* vehicle) {
    Telemetry::Quaternion quaternion;

    //Sets up the requested broadcast frequencies - specifically 100Hz on Quaternion
    setBroadcastFrequency(vehicle);

    //Request the desired angle
    std::cout << "Insert angle please " << std::endl;
    int requestangle;
    std::cin >> requestangle;

    float32_t degStart;
    float32_t offset;
    float32_t yaw = 0;
    int counter = 0;

    //Sets a start angle
    degStart = QtoDEG(vehicle);
    printf("degStart: %f, yaw: %f\n", degStart, yaw);

    while (1) {
        degStart = QtoDEG(vehicle);

        //degStart = angle;
        vehicle->control->positionAndYawCtrl(0, 0, 3, yaw);
        offset = fabs(fabs(degStart) - fabs(yaw));
        if (offset < 0.01) {
            counter++;
            printf("c: %i\n", counter);
        } else {
            counter = 0;
            //printf("degStart: %f, yaw: %f\n", angle, yaw);
        }

        if (counter > 10) {
            break;
        }

        usleep(10000);
    }
    printf("degStart: %f, yaw: %f\n", degStart, yaw);
    counter = 0;

    printf("degStart: %f, yaw: %f\n", degStart, yaw);
    sleep(2);

    float32_t degree;
    float32_t degTarget = degStart - requestangle;
    int timestepInMS = 10;
    printf("degStart: %f, degTarget: %f\n", degStart, degTarget);

    int time = 0;
    while (1) {
        vehicle->control->positionAndYawCtrl(0, 0, 3, degTarget);
        degree = QtoDEG(vehicle);
        std::cout << time << "," << fabs(degree) << "\n";

        offset = fabs(fabs(degTarget) - fabs(degree));
        if (offset < 0.01) {
            counter++;
        } else {
            counter = 0;
        }

        if (counter > 100) {
            break;
        }

        time = time + timestepInMS;
        usleep(timestepInMS * 1000);
    }
}

float32_t QtoDEG(Vehicle* vehicle) {
    quaternion = vehicle->broadcast->getQuaternion();
    double t1 = +2.0 * (quaternion->q1 * quaternion->q2 + quaternion->q0 * quaternion->q3);
    double t0 = -2.0 * (quaternion->q2 * quaternion->q2 + quaternion->q3 * quaternion->q3) + 1.0;
    float32_t angle = atan2(t1, t0) * 180 / M_PI;
    return angle;
}

void setBroadcastFrequency(Vehicle* vehicle) {
    enum FREQ {
        FREQ_0HZ = 0,
        FREQ_1HZ = 1,
        FREQ_10HZ = 2,
        FREQ_50HZ = 3,
        FREQ_100HZ = 4,
        FREQ_200HZ = 6,
        FREQ_400HZ = 7,
        FREQ_HOLD = 5,
    };

    const int TIMEOUT = 20;
    uint8_t freq[16];
    /* Channels definition for M100
   * 0 - Timestamp
   * 1 - Attitude Quaterniouns
   * 2 - Acceleration
   * 3 - Velocity (Ground Frame)
   * 4 - Angular Velocity (Body Frame)
   * 5 - Position
   * 6 - Magnetometer
   * 7 - RC Channels Data
   * 8 - Gimbal Data
   * 9 - Flight Status
   * 10 - Battery Level
   * 11 - Control Information
   */
    freq[0] = FREQ_10HZ;
    freq[1] = FREQ_100HZ;
    freq[2] = FREQ_100HZ;
    freq[3] = FREQ_10HZ;
    freq[4] = FREQ_100HZ;
    freq[5] = FREQ_100HZ;
    freq[6] = FREQ_100HZ;
    freq[7] = FREQ_0HZ;
    freq[8] = FREQ_0HZ;
    freq[9] = FREQ_0HZ;
    freq[10] = FREQ_0HZ;
    freq[11] = FREQ_100HZ;

    ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreq(freq, TIMEOUT);
}
