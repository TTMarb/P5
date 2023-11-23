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
 * The trasnceiver search stage initiates the antenna_dft process, which in turn 
 * reads the antenna, calculates the DFT, and transmits via a UNIX domain socket.
 * If the input signal threshold is reached, the waypoint mission stops and starts
 * the coarse search stage. 
 * 
 * 
 *
 */

#include <cmath>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include "transceiver_search.hpp"

// Path for UNIX domain socket
#define SERVER_PATH "/tmp/unix_sock.server"
// Buffer for antenna data
#define BUFFER_SIZE 10
float buf[BUFFER_SIZE];

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

int main(int argc, char** argv) {

    /********* INITIALIZE DFT PROCESS *********/

    // Quick of the start process of coarse search!
    pid_t coarseSearch;
    coarseSearch = fork(); // Fork the parent process to start new process
    char pathTest[] = "/home/ubuntu/Documents/P5/Onboard-SDK/build/bin/coarse_search";
    char paramTest[] = "UserConfig.txt" startProcess(antennaPID, path, param);

    pid_t antennaPID;
    antennaPID = fork(); // Fork the parent process to start new process
    char path[] = "/home/ubuntu/Documents/P5/Onboard-SDK/build/bin/antenna_dft";
    startProcess(antennaPID, path, NULL);

    /********* WAYPOINT MISSION *********/

    int functionTimeout = 60;
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
    int responseTimeout = 60;
    uint8_t numWaypoints;
    uint8_t errorFlag = 0;
    // Sets S and W parameters for transceiver search
    float64_t latM; // Y distance per module
    float64_t lonM; // X distance per module
    float avLength, avWidth, searchWidth, pathLength;

    //Delay before stopping mission - purely for testing purposes
    int delayBeforeStop;

    while (1) {
        // Display interactive prompt, that allows user to select mission parameters
        std::cout << "Beginning waypoint mission.\n"
                  << "Input length of avalanche (0-200m): " << std::endl;
        std::cin >> avLength;
        if (avLength <= 200.0 && avLength > 0.0) {
        } else {
            std::cout << "Error: Length is not within interval. Please try again.\n";
            errorFlag = 1;
            break;
        }
        std::cout << "Input width of avalanche (0-50m): " << std::endl;
        std::cin >> avWidth;
        if (avWidth <= 50.0 && avWidth > 0.0) {
        } else {
            std::cout << "Error: Width is not within interval. Please try again.\n";
            errorFlag = 1;
            break;
        }

        std::cout << "About to sleep for 30 secs" << std::endl;
        for (int i = 0; i < 30; i++) {
            std::cout << i << std::endl;
            sleep(1);
        }
        std::cout << "Have slept for 30 secs" << std::endl;

        // Calculations for avalanche size inputs
        searchWidth = 10;
        avLength = avLength - (2 * searchWidth);
        avWidth = avWidth - (2 * searchWidth);
        lonM = 2 * searchWidth;
        latM = avWidth;

        numWaypoints = ceilf(2 * (avLength / lonM)) + 2;
        if (numWaypoints > 0.0 && numWaypoints <= 255.0) // Only allow 255 waypoints
        {
            numWaypoints = static_cast<int>(numWaypoints); // Make the number of waypoints an integer
            pathLength = (numWaypoints / 2) * latM + ((numWaypoints / 2) - 1) * lonM;
            std::cout << "The number of waypoints is " << +numWaypoints << std::endl;
            std::cout << "The path length is " << pathLength << " m\n";
            break;
        } else {
            std::cout << "Error: Number of waypoints is outside interval. Please try again.\n";
            errorFlag = 1;
            break;
        }
    }

    if (errorFlag == 0) {
        std::cout << "Starting mission.\n";
        std::cout << "The number of waypoints is " << numWaypoints << std::endl;
        runWaypointMission(vehicle, numWaypoints, responseTimeout, latM, lonM);
    }

    /********* START OF DOMAIN SOCKET *********/

    int client_sock, rc;
    uint32_t len;
    struct sockaddr_un client_sockaddr, server_adress;
    /* 
    * Clear the whole struct to avoid portability issues,
    * where some implementations have non-standard fields. 
    */
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, sizeof(float) * BUFFER_SIZE);

    // Create a socket
    client_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_sock == -1) {
        printf("SOCKET ERROR\n");
        exit(EXIT_FAILURE);
    }

    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, SERVER_PATH);
    len = sizeof(client_sockaddr);
    unlink(SERVER_PATH);
    rc = bind(client_sock, (struct sockaddr*)&client_sockaddr, len);
    if (rc == -1) {
        printf("BIND ERROR\n");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    int timeOutSet = 0;
    printf("Waiting to receive...\n");
    while (1) {
        // Stay in a blocked state until data is received
        rc = recvfrom(client_sock, buf, sizeof(float) * BUFFER_SIZE, 0, (struct sockaddr*)&server_adress, &len);

        if (rc == -1) {
            if (timeOutSet == 0) {
                printf("RECEIVE ERROR\n");
                timeOutSet = 1;
            }
        } else {
            // Error message if connection was briefly lost
            if (timeOutSet == 1) {
                printf("\nConnection restablished. Receiving data...\n");
                timeOutSet = 0;
            }
            // Iterate through buffer to read data values
            int i;
            for (i = 0; i < BUFFER_SIZE; i++) {
                if (buf[i] >= 9.7) { // NB! edit to desired threshold
                    close(client_sock);
                    stopMission(vehicle, responseTimeout, 0); // Stop waypoint mission if threshold is reached
                    printf("Stopping waypoint mission...\n");
                    printf("Starting coarse search!\n");
                    pid_t coarsePID;
                    coarsePID = fork(); /* Fork the parent process to start new process*/
                    char path[] = "/home/ubuntu/Documents/P5/Onboard-SDK/build/bin/coarse_search";
                    char param[] = "UserConfig.txt";
                    startProcess(antennaPID, path, param);
                    return 0;
                    exit(EXIT_SUCCESS); // Exit process
                }
            }
        }
    }
}