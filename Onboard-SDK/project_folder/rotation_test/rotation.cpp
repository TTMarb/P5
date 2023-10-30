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

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void getRotation(Vehicle* vehicle) {

    Telemetry::Quaternion quaternion;
    Telemetry::Status status;
    Telemetry::Vector3f velocity;

    const int TIMEOUT = 20;

    // Re-set Broadcast frequencies to their default values
    ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreqDefaults(TIMEOUT);

    // Print in a loop for 2 seconds
    while (1) {
        // Matrice 100 broadcasts only flight status
        status = vehicle->broadcast->getStatus();
        quaternion = vehicle->broadcast->getQuaternion();
        velocity = vehicle->broadcast->getAngularRate();
        std::cout << "-------\n";
        std::cout << "Flight Status                         = " << (unsigned)status.flight << "\n";
        std::cout << "Attitude Quaternion   (w,x,y,z)       = " << quaternion.q0 << ", " << quaternion.q1 << ", "
                  << quaternion.q2 << ", " << quaternion.q3 << "\n";
        std::cout << "Angular Rate          (x,y,z)         = " << velocity.x << ", " << velocity.y << ", "
                  << velocity.z << "\n";
        std::cout << "-------\n";

        sleep(1);
    }
}
