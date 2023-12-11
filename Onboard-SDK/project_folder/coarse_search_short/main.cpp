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
 */

#include <cmath>
#include "coarse_search.hpp"

int timecounterMilliseconds = 0;

int main(int argc, char** argv, char** envp) {
    // Setup OSDK.
    LinuxSetup linuxEnvironment(argc, argv);

    
    // Starts the antenna_dft process
    pid_t antennaPID, wpid;
    antennaPID = fork(); // Fork the parent process to start new process
    char path[] = "/home/ubuntu/Documents/P5/Onboard-SDK/build/bin/antenna_dft";
    startProcess(antennaPID, path, NULL, 0);
    int pStatus = 0;

    //Initialize vehicle
    Vehicle* vehicle = linuxEnvironment.getVehicle();
    if (vehicle == NULL) {
        std::cout << "Vehicle not initialized, exiting.\n";
        return -1;
    }

    int functionTimeout = 60;
    UAVtakoff(vehicle, functionTimeout);

    // Setup variables for use
    FIO fileIO = FIO();
    fileIO.changeActiveFile("test.txt");
    fileIO.createFile();
    fileIO.write2file("H, alg, vel, pos.longitude, pos.latitude, UAVangle, A1, A2");
    float alg, vel, A1, A2, H, prevH, sampleFrequency, velKp;
    int mult;
    A1 = 0;
    A2 = 0;
    sampleFrequency = 100;
    velKp = 0.25;
    PIcontroller yawRate = PIcontroller(0.15, 0, sampleFrequency); //0.02
    PIcontroller vX = PIcontroller(velKp, 0, sampleFrequency);
    PIcontroller vY = PIcontroller(velKp, 0, sampleFrequency);

    sock soc = sock();

    while (1) {
        // Transmit data to antenna_dft process
        DJI::OSDK::Telemetry::GlobalPosition pos;
        pos = vehicle->broadcast->getGlobalPosition(); // Get the current GNSS position
        float UAVangle = QtoDEG(vehicle);              // Get the current UAS angle

        soc.sendit(pos.longitude, pos.latitude, UAVangle);

        // Stay in a blocked state until data is received
        if (soc.receive(&A1, &A2)) {
            H = calcH(vehicle, &A1, &A2, &H);
            alg = calcAlg(vehicle, &A1, &A2, &H);
            vel = calcVel(vehicle, &H, &prevH, &mult, velKp);
            std::cout << "\tH: " << H << " Alg: " << alg << " Vel: " << vel << std::endl;
            controlVehicle(vehicle, &vel, &alg, &fileIO, &yawRate, &vX, &vY, sampleFrequency, &timecounterMilliseconds);
            std::cout << "\tyawRate: " << yawRate.PIvalue << " vX: " << vX.PIvalue << " vY: " << vY.PIvalue
                      << std::endl;
            //Break statement - Within 2x of the target
            if (H > (140)) { //<- Within 3 m
                //Stops the UAV
                vehicle->control->velocityAndYawRateCtrl(0, 0, 0, 0);
                std::cout << "Target found! \n";
                break;
            }
            fileIO.write2file(std::to_string(H) + "," + std::to_string(alg) + "," + std::to_string(vel) + ","
                              + std::to_string(pos.longitude) + "," + std::to_string(pos.latitude) + ","
                              + std::to_string(UAVangle) + "," + std::to_string(A1) + "," + std::to_string(A2));
            prevH = H;
        }
    }
    close(soc.client_sock);

    //Set the bool to true to land the UAV, false to stay in the air
    UAVstop(vehicle, true, functionTimeout);
    std::cout << "Stopping coarse_search" << std::endl;

    // Get the antenna_dft process pid
    std::string envPID;
    while (*envp != nullptr) {
        envPID += *envp;
        envp++;
    }
    int pidNum = std::stoi(envPID);
    pid_t pidTemp = static_cast<pid_t>(pidNum);
    std::cout << "antenna_dft pid: " << pidNum << std::endl;
    // Terminate the process
    if (kill(pidTemp, SIGTERM) == 0) {
        std::cout << "Terminating antenna_dft" << std::endl;
    } else {
        perror("kill");
    }
    exit(EXIT_SUCCESS); //Exit the coarse_search process
    return 0;
}