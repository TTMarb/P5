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
    vehicle->obtainCtrlAuthority(functionTimeout);

    // Setup variables for use

    std::cout << "Program about to start! \n" << std::endl;
    

    //Request the desired angle
    std::cout << "Insert angle please " << std::endl;
    int requestangle;
    std::cin >> requestangle;
    std::string filename = "Filename.csv";

    getRotation(vehicle, requestangle, filename);
    std::cout << "Program ended!" << std::endl;
}