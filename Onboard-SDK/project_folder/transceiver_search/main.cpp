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
    // Sets S and W parameters for transceiver search
    float latM; // X distance per module
    float lonM; // Y distance per module
    float avLength, avWidth, searchWidth, pathLength;

    //Delay before stopping mission - purely for testing purposes
    int delayBeforeStop;

    while (1) {
        // Display interactive prompt, that allows user to select mission parameters
        std::cout << "Beginning waypoint mission.\n"
                  << "Input length of avalanche (0-200m): " << std::endl;
        std::cin >> avLength;
        if (avLength <= 200.0 && avLength > 0.0) {
            std::cout << "The length is: " << avLength << std::endl;
        } else {
            std::cout << "Error: Length is not within interval. Please try again.\n";
            break;
        }
        std::cout << "Input width of avalanche (0-50m): " << std::endl;
        std::cin >> avWidth;
        if (avWidth <= 50.0 && avWidth > 0.0) {
            std::cout << "The width is: " << avWidth << std::endl;
        } else {
            std::cout << "Error: Width is not within interval. Please try again.\n";
            break;
        }
        // Calculations for avalanche size inputs
        searchWidth = 10;
        avLength = avLength - searchWidth;
        avWidth = avWidth - searchWidth;

        latM = avWidth;
        lonM = 2 * searchWidth;

        float turns = ceilf(avLength / lonM);
        if (turns * 2 > 0 && turns * 2 <= 255) // Only allow 255 waypoints
        {
            numWaypoints = static_cast<uint8_t>(turns * 2); // Make the number of waypoints an integer
            pathLength = (turns + 1) * latM + turns * lonM;
            std::cout << "The number of waypoints is " << +numWaypoints << std::endl;
            std::cout << "The path length is " << pathLength << " m\n";
        } else {
            std::cout << "Error: Number of waypoints is outside. Please try again.\n";
            break;
        }
        runWaypointMission(vehicle, numWaypoints, responseTimeout, latM, lonM);
        //delayBeforeStop = 360;
        //stopMission(vehicle, responseTimeout, delayBeforeStop);
    }
    return 0;
}