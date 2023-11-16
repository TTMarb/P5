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
#include "rotation.hpp"
#include "FIO.h"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

int main(int argc, char** argv) {
    FIO fileIO = FIO();
    int functionTimeout = 20;
    // Setup OSDK.
    LinuxSetup linuxEnvironment(argc, argv);
    //Initialize vehicle
    Vehicle* vehicle = linuxEnvironment.getVehicle();
    if (vehicle == NULL) {
        std::cout << "Vehicle not initialized, exiting.\n";
        return -1;
    }
    



    // Obtain Control Authority
    
    std::cout << "About to take control \n";
    vehicle->obtainCtrlAuthority(functionTimeout);
    
    std::cout << "About to sleep for 30 secs \n";
    for(int i = 0 ; i < 30; i++){
        std::cout << i << std::endl;
        sleep(1);
    }

    std::cout << "Arm motor \n";
    ACK::ErrorCode armAck = vehicle->control->armMotors(functionTimeout);
    if (ACK::getError(armAck)) {
        ACK::getErrorCodeMessage(armAck, __func__);
    }
    std::cout << "About to take off \n";
    std::cout << "Have slept for 30 secs \n";

    ACK::ErrorCode takeoffAck = vehicle->control->takeoff(functionTimeout);
    if (ACK::getError(takeoffAck)) {
        ACK::getErrorCodeMessage(takeoffAck, __func__);
    }

    sleep(5);

    // Setup variables for use

    std::cout << "Program about to start! \n" << std::endl;
    

    //Request the desired angle
    std::cout << "Beginning tests" << std::endl;

    std::vector<int> testangles={1,5,15,45,145};

    for (int TA : testangles){
        std::cout << "Going for test angle: " << TA << std::endl;
        std::string filename = "deg" + std::to_string(TA) + ".csv";
        getRotation(vehicle, TA, filename, fileIO);
    }

    ACK::ErrorCode landAck = vehicle->control->land(functionTimeout);
    if (ACK::getError(landAck)) {
        ACK::getErrorCodeMessage(landAck, __func__);
    }

    std::cout << "Arm motor \n";
    ACK::ErrorCode disarmAck = vehicle->control->disarmMotors(functionTimeout);
    if (ACK::getError(disarmAck)) {
        ACK::getErrorCodeMessage(disarmAck, __func__);
    }
    std::cout << "About to take off \n";
    std::cout << "Have slept for 30 secs \n";





    std::cout << "Program ended!" << std::endl;
}