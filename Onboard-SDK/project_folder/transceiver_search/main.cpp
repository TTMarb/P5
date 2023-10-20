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

#include "transceiver_search.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

int main(int argc, char** argv) {
    // Initialize variables
    int functionTimeout = 1;

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

    int responseTimeout = 1;

    uint8_t numWaypoints;
    float searchWidth = 10;
    // Sets S and W parameters for transceiver search
    float S; //Width per module
    float W; //Heigth per module

    float avLength, avWidth;

    //Delay before stopping mission - purely for testing purposes
    int delayBeforeStop;

    // Display interactive prompt, that allows user to select mission parameters
    std::cout << "| Beginning waypoint mission.        \t|"
              << "\n"
              << "| Input length of avalanche:         \t|" << std::endl;
    std::cin >> avLength;
    std::cout << "| S has been selected as: " << avLength << "\t|\n"
              << "|            Choose W                \t|" << std::endl;
    std::cin >> W;
    std::cout << "| W has been selected as: " << W << "\t|" << std::endl;

    // Calculations for avalanche size inputs
    searchWidth

        numWaypoints = 10;
    //delayBeforeStop = 360;

    runWaypointMission(vehicle, numWaypoints, responseTimeout, S, W);
    //stopMission(vehicle, responseTimeout, delayBeforeStop);

    return 0;
}