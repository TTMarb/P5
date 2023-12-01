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

#include "velopcity.hpp"
#include "FIO.h"
#include <dji_broadcast.hpp>
#include <dji_telemetry.hpp>
#include <fstream>
#include <iostream>
#define _USE_MATH_DEFINES

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void getRotation(Vehicle* vehicle, float32_t targetVelocity, std::string filename, FIO fileIO) {
    fileIO.changeActiveFile(filename);
    fileIO.createFile();

    //Sets up the requested broadcast frequencies - specifically 100Hz on Quaternion
    setBroadcastFrequency(vehicle);

    //Initialises different parameters used for the rotation
    float32_t currVelocity;
    int counter = 0;

    //Print 2 confirm starting angle

    sleep(2);
    //Creates timing for plotting
    int time = 0;
    int timestepInMS = 10;
    while (isTargetHit(vehicle, targetVelocity, &currVelocity, &counter, 10)) {
        time = time + timestepInMS;
        std::string data = std::to_string(time) + "," + std::to_string(fabs(currVelocity));
        fileIO.write2file(data);
        std::cout << data << "\n";
        usleep(timestepInMS * 1000);
    }
}

float32_t getVelocity(Vehicle* vehicle) {
    //This function converts the quaternion to degrees
    Telemetry::Vector3f vel;
    vel = vehicle->broadcast->getVelocity();
    //Largely based on a mix of https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_(in_3-2-1_sequence)_conversion
    //and the flight-control sample
    return vel.x;
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

bool isTargetHit(Vehicle* vehicle, float32_t targetVelocity, float32_t* currVelocity, int* counter, int counterGoal) {
    //Main loop
    //Asks the control to move to target angle
    ///@todo: Change to velocity control
    vehicle->control->velocityAndYawCtrol(targetVelocity, 0, 0, 0);
    //Gets the current angle og the system
    *currVelocity = getVelocity(vehicle);
    //Calculates the offset of the two angles
    float32_t offset = fabs(fabs(targetVelocity) - fabs(*currVelocity));
    //Counts to make sure the system is stable - Setteling time and such
    if (offset < 0.1) {
        *counter = *counter + 1;
    } else {
        *counter = 0;
    }

    return (*counter < counterGoal);
}
