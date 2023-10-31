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
#define _USE_MATH_DEFINES

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

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

void getRotation(Vehicle* vehicle) {

    Telemetry::Quaternion quaternion;
    Telemetry::Status status;
    Telemetry::Vector3f velocity;
    Telemetry::Mag magnet;

    const int TIMEOUT = 20;
    double yawInRad;
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

    // Re-set Broadcast frequencies to their default values
    //ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreqDefaults(TIMEOUT);
    ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreq(freq, TIMEOUT);
    magnet = vehicle->broadcast->getMag();

    // Print in a loop for 2 seconds

    float32_t degStart;
    magnet = vehicle->broadcast->getMag();
    degStart = XYtoDEG(magnet.x, magnet.y);
    float32_t yaw = 0;
    sleep(1);
    printf("degStart: %f, yaw: %f\n", degStart, yaw);
    while (fabs(degStart + yaw) > 0.01) {
        printf("degStart: %f, yaw: %f\n", degStart, yaw);
        magnet = vehicle->broadcast->getMag();
        degStart = XYtoDEG(magnet.x, magnet.y);
        vehicle->control->positionAndYawCtrl(0, 0, 3, yaw);
        usleep(100000);
    }
    printf("degStart: %f, yaw: %f\n", degStart, yaw);
    sleep(2);

    float32_t degree;
    float32_t degTarget = degStart - 1;
    printf("degStart: %f, degTarget: %f\n", degStart, degTarget);

    int time = 0;
    int counter;
    while (1) {
        vehicle->control->positionAndYawCtrl(0, 0, 3, degTarget); //1.57); //yaw + 90.0);
        // Matrice 100 broadcasts only flight status
        status = vehicle->broadcast->getStatus();
        quaternion = vehicle->broadcast->getQuaternion();
        velocity = vehicle->broadcast->getAngularRate();
        magnet = vehicle->broadcast->getMag();
        degree = XYtoDEG(magnet.x, magnet.y);

        //std::cout << "\t magnet.x: " << magnet.x << "\n";
        //std::cout << "\t magnet.y: " << magnet.y << "\n";
        //std::cout << "\t D meas: " << degree << ", Changed: " << degree - degStart << " :)\n";
        std::cout << time << "," << degree << "\n";
        if (fabs(fabs(degTarget) - fabs(degree)) < 0.1) {
            counter++;
        } else {
            counter = 0;
        }

        if (counter > 200) {
            break;
        }
        /* else {
            std::cout << "\t magnet.y = 0"
                      << "\n";
        }*/
        /*
        //yawInRad = toEulerAngle((static_cast<void*>(&quaternion))).z / DEG2RAD;
        std::cout << "-------\n";
        std::cout << "Flight Status                         = " << (unsigned)status.flight << "\n";
        std::cout << "Abs of Yaw:                           = " << quaternion.q0 << "\n";
        std::cout << "Angular Rate in z direction:          = " << velocity.z << "\n";
        //std::cout << "YawInRad:                             = " << yawInRad << "\n";
        std::cout << "Magnetometer  (x,y,z)                 = " << magnet.x << ", " << magnet.y << ", " << magnet.z
                  << "\n";

        //Vi får en floating point exception af det her :(
        std::cout << "A circle: " << degree << ", asin(x/1500) = " << sin(magnet.x / 1500.0)
                  << ", acos(y/1500) = " << cos(magnet.y / 1500.0) << "\n";
        std::cout << "Yaw new: " << yaw << "\n";
        std::cout << "-------\n";*/

        time = time + 5;
        usleep(5000);
    }
}

float32_t XYtoDEG(int ix, int iy) {
    float32_t degStart;
    if (iy != 0) {
        //float32_t x = ix / 1500.0;
        //float32_t y = iy / 1500.0;
        float32_t x = ix;
        float32_t y = iy;
        degStart = atan2(y, x) * (180 / M_PI);
    }
    return degStart;
}