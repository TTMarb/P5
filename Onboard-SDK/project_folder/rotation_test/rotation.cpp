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
#include "FIO.h"
#include <dji_broadcast.hpp>
#include <dji_telemetry.hpp>
#include <fstream>
#include <iostream>
#define _USE_MATH_DEFINES

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void getRotation(Vehicle* vehicle, int setAngle, std::string filename, FIO fileIO) {
    fileIO.changeActiveFile(filename);
    fileIO.createFile();

    //Sets up the requested broadcast frequencies - specifically 100Hz on Quaternion
    setBroadcastFrequency(vehicle);

    //Initialises different parameters used for the rotation
    float32_t currAngle;
    float32_t offset;
    float32_t targetAngle = 0;
    int counter = 0;

    //Sets a start angle
    currAngle = QtoDEG(vehicle);
    printf("currAngle: %f, targetAngle: %f\n", currAngle, targetAngle);

    //Step 1: set the current angle to the start angle - typically 0.
    while (isTargetHit(vehicle, targetAngle, &currAngle, &counter, 10)) {
        usleep(10000);
    }
    //Print 2 confirm starting angle
    printf("currAngle: %f, targetAngle: %f\n", currAngle, targetAngle);

    sleep(2);
    //Sets a new target angle
    targetAngle = currAngle - setAngle;
    //Creates timing for plotting
    int time = 0;
    int timestepInMS = 10;
    while (isTargetHit(vehicle, targetAngle, &currAngle, &counter, 10)) {
        time = time + timestepInMS;
        std::string data = std::to_string(time) + "," + std::to_string(fabs(currAngle));
        fileIO.write2file(data);
        std::cout << data << "\n";
        usleep(timestepInMS * 1000);
    }
}

float32_t QtoDEG(Vehicle* vehicle) {
    //This function converts the quaternion to degrees
    Telemetry::Quaternion quaternion;
    quaternion = vehicle->broadcast->getQuaternion();
    //Largely based on a mix of https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_(in_3-2-1_sequence)_conversion
    //and the flight-control sample
    double t1 = +2.0 * (quaternion.q1 * quaternion.q2 + quaternion.q0 * quaternion.q3);
    double t0 = -2.0 * (quaternion.q2 * quaternion.q2 + quaternion.q3 * quaternion.q3) + 1.0;
    //180/M_pi is to convert from radians to degrees
    float32_t angle = atan2(t1, t0) * 180 / M_PI;
    return angle;
}

void setBroadcastFrequency(Vehicle* vehicle) {
    //To ensure a faster response, the broadcast frequency is set to 100Hz for Quaternion
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

    //Timeout is for acknowledgement
    const int TIMEOUT = 20;
    //Initialises the frequency array - 16 is the given lenght by documentation
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
    freq[0] = FREQ_100HZ;
    freq[1] = FREQ_100HZ;
    freq[2] = FREQ_100HZ;
    freq[3] = FREQ_100HZ;
    freq[4] = FREQ_100HZ;
    freq[5] = FREQ_100HZ;
    freq[6] = FREQ_100HZ;
    freq[7] = FREQ_100HZ;
    freq[8] = FREQ_100HZ;
    freq[9] = FREQ_100HZ;
    freq[10] = FREQ_100HZ;
    freq[11] = FREQ_100HZ;

    ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreq(freq, TIMEOUT);
}

bool isTargetHit(Vehicle* vehicle, float32_t targetAngle, float32_t* currAngle, int* counter, int counterGoal) {
    //Main loop
    //Asks the control to move to target angle
    vehicle->control->positionAndYawCtrl(0, 0, 3, targetAngle);
    //Gets the current angle og the system
    *currAngle = QtoDEG(vehicle);
    //Calculates the offset of the two angles
    float32_t offset = fabs(fabs(targetAngle) - fabs(*currAngle));
    //Counts to make sure the system is stable - Setteling time and such
    if (offset < 0.01) {
        *counter = *counter + 1;
    } else {
        *counter = 0;
    }
    //Debug print statement
    //std::cout << *counter << " < " << counterGoal << ": " << (*counter < counterGoal) << std::endl;
    //If counter is above counterGoal, the while loops will be broken
    return (*counter < counterGoal);
}