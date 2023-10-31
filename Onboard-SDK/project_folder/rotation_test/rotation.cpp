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
#include <dji_telemetry.hpp>
#define _USE_MATH_DEFINES

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void getRotation(Vehicle* vehicle) {

    Telemetry::Quaternion quaternion;
    Telemetry::Status status;
    Telemetry::Vector3f velocity;
    Telemetry::Mag magnet;

    const int TIMEOUT = 20;
    double yawInRad;

    // Re-set Broadcast frequencies to their default values
    ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreqDefaults(TIMEOUT);
    ACK::ErrorCode wack = vehicle->broadcast->setBroadcastFreq(FREQ::FREQ_400Hz, TIMEOUT);
    magnet = vehicle->broadcast->getMag();

    // Print in a loop for 2 seconds
    int yaw = -180;
    while (yaw < 180) {
        // Matrice 100 broadcasts only flight status
        status = vehicle->broadcast->getStatus();
        quaternion = vehicle->broadcast->getQuaternion();
        velocity = vehicle->broadcast->getAngularRate();
        magnet = vehicle->broadcast->getMag();
        std::cout << "\t magnet.x: " << magnet.x << "\n";
        std::cout << "\t magnet.y: " << magnet.y << "\n";
        if (magnet.y != 0) {
            float32_t x = magnet.x / 1500.0;
            float32_t y = magnet.y / 1500.0;
            float32_t yawInRad = atan2(y, x);
            /*if (x < 0) {
                yawInRad = yawInRad + M_PI;
            }*/
            std::cout << "\t Degrees: " << yawInRad * (180 / M_PI) << " :)\n";
        } else {
            std::cout << "\t magnet.y = 0"
                      << "\n";
        }
        //yawInRad = toEulerAngle((static_cast<void*>(&quaternion))).z / DEG2RAD;
        std::cout << "-------\n";
        std::cout << "Flight Status                         = " << (unsigned)status.flight << "\n";
        std::cout << "Abs of Yaw:                           = " << quaternion.q0 << "\n";
        std::cout << "Angular Rate in z direction:          = " << velocity.z << "\n";
        //std::cout << "YawInRad:                             = " << yawInRad << "\n";
        std::cout << "Magnetometer  (x,y,z)                 = " << magnet.x << ", " << magnet.y << ", " << magnet.z
                  << "\n";

        //Vi får en floating point exception af det her :(
        /*std::cout << "A circle: " << degree << ", asin(x/1500) = " << sin(magnet.x / 1500.0)
                  << ", acos(y/1500) = " << cos(magnet.y / 1500.0) << "\n";*/
        std::cout << "Yaw new: " << yaw << "\n";
        std::cout << "-------\n";

        vehicle->control->positionAndYawCtrl(0, 0, 3, yaw);

        yaw = yaw + 1;
        usleep(100000);
    }
}
