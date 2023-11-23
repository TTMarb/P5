/*! @file missions/main.cpp
 *  @date Jun 05 2017

 *  @brief
 *  Flight control application for the initial transceiver seach.
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

#include <cmath>
#include "coarse_search.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

int main(int argc, char** argv) {
    // Setup OSDK.
    LinuxSetup linuxEnvironment(argc, argv);
    //Initialize vehicle
    Vehicle* vehicle = linuxEnvironment.getVehicle();
    if (vehicle == NULL) {
        std::cout << "Vehicle not initialized, exiting.\n";
        return -1;
    }
    
    setBroadcastFrequency(vehicle);
    Telemetry::Status status = vehicle->broadcast->getStatus();

    
    int functionTimeout = 60;

    std::cout << "About to take control \n";
    vehicle->obtainCtrlAuthority(functionTimeout);
    if(status.flight < 2){
        sleep(5);
        std::cout << "Preparing drone" << std::endl;

        std::cout << "Arm motor \n";
        ACK::ErrorCode armAck = vehicle->control->armMotors(functionTimeout);
        if (ACK::getError(armAck)) {
            ACK::getErrorCodeMessage(armAck, __func__);
        }

        std::cout << "About to take off \n";
        sleep(5);

        ACK::ErrorCode takeoffAck = vehicle->control->startTakeoffSync(functionTimeout);
        if (ACK::getError(takeoffAck)) {
            ACK::getErrorCodeMessage(takeoffAck, __func__);
        }

        std::cout << "Took off \n";
        sleep(10);
    } else{
        std::cout << "Drone already in air" << std::endl;
    }
    sleep(5);

    // Setup variables for use
    tellMeAboutTheData(vehicle);

    return 0;
}