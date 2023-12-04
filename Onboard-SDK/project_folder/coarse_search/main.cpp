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

// Path for UNIX domain socket
#define SERVER_PATH      "/tmp/unix_sock.server"
#define CLIENT_PATH      "/tmp/unix_sock.client"

// Buffer for sending the gps info for the data generator in antenna_dft
#define SEND_BUFFER_SIZE 3
double sendBuf[SEND_BUFFER_SIZE]; // Contains longitude, latitude, and angle

// Buffer for receiving antenna data
#define BUFFER_SIZE 2
float buf[BUFFER_SIZE]; // Contains only A1 and A2 data at a time
int timecounterMilliseconds = 0;

int main(int argc, char** argv) {
    // Setup OSDK.
    LinuxSetup linuxEnvironment(argc, argv);
    //Initialize vehicle
    Vehicle* vehicle = linuxEnvironment.getVehicle();
    if (vehicle == NULL) {
        std::cout << "Vehicle not initialized, exiting.\n";
        return -1;
    }
    
    int functionTimeout = 60;
    UAVland(vehicle,functionTimeout);
    
    // Setup variables for use
    FIO fileIO = FIO();
    fileIO.changeActiveFile("test.txt");
    fileIO.createFile();
    //DataFaker df = DataFaker();
    //initializeFake(vehicle, &df, &fileIO);
    float alg, vel, A1, A2, H, prevH, sampleFrequency;
    int cnt, mult;
    A1 = 0;
    A2 = 0;
    sampleFrequency = 100;
    PIcontroller yawRate = PIcontroller(0.75, 0.02, sampleFrequency);
    PIcontroller vX = PIcontroller(0.05, 0, sampleFrequency);
    PIcontroller vY = PIcontroller(0.05, 0, sampleFrequency);

    /********* START OF DOMAIN SOCKET *********/

    int client_sock, rc;
    uint32_t len;
    struct sockaddr_un client_sockaddr, server_sockaddr;

    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));

    // Create a socket
    client_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Get socket path for both server and client
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);

    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);

    // Bind the client to the client filename
    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr*)&client_sockaddr, sizeof(client_sockaddr));
    if (rc == -1) {
        perror("bind");
        close(client_sock);
        exit(EXIT_FAILURE);
    }
    // Connect client to server filename
    rc = connect(client_sock, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
    if (rc == 1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    int timeOutSet = 0;

    /*********END OF DOMAIN SOCKET *********/

    while (1) {
        // Transmit data to antenna_dft process
        Telemetry::GlobalPosition pos;
        pos = vehicle->broadcast->getGlobalPosition(); // Get the current GNSS position
        float ang = QtoDEG(vehicle);                   // Get the current UAS angle

        sendBuf[0] = pos.longitude;
        sendBuf[1] = pos.latitude;
        sendBuf[2] = ang;

        rc = send(client_sock, sendBuf, sizeof(double) * SEND_BUFFER_SIZE, 0);
        if (rc == -1) {
            perror("send");
        } else {
            // Data is sent here!
            printf("Sending buffer %f, %f, %f\n", sendBuf[0], sendBuf[1], sendBuf[2]);
        }

        // Stay in a blocked state until data is received
        rc = recv(client_sock, buf, sizeof(float) * BUFFER_SIZE, 0);
        if (rc == -1) {
            if (timeOutSet == 0) {
                perror("recv");
                timeOutSet = 1;
            }
        } else {
            // Error message if connection was briefly lost
            if (timeOutSet == 1) {
                printf("\nConnection restablished. Receiving data...\n");
                timeOutSet = 0;
            }
            printf("Received %f %f", buf[0], buf[1]);
            A1 = buf[0];
            A2 = buf[1];
            /*
            df.Fake(vehicle, fileIO, true);
            A1 = df.A1;
            A2 = df.A2;
            */

            controlVehicle(vehicle, &vel, &alg, &fileIO, &yawRate, &vX, &vY, sampleFrequency, &timecounterMilliseconds);

            //Break statement - Within 2x of the target
            if (H > (4096 * 10)) { //<- Within 0.5 m :P
                //Stops the UAV
                vehicle->control->velocityAndYawRateCtrl(0, 0, 0, 0);
                std::cout << "Target found! \n";
                break;
            }
            prevH = H;
            cnt++;
        }
    }

    //Set the bool to true to land the UAV, false to stay in the air
    UAVstop(vehicle,true,functionTimeout);
    return 0;
}