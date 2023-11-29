/* Process for reading antenna outputs and performing DFT.
 * The calculations are sent using a UNIX domain socket.
 *  
 */

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include "antenna.h"
#define EARTH_RADIUS     (double)6378137.0
#define searchRadius     10

/* Path for UNIX domain socket */
#define SERVER_PATH      "/tmp/unix_sock.server"

// Buffer for sending the gps info for the data generator in antenna_dft
#define RECV_BUFFER_SIZE 3
double recvBuf[RECV_BUFFER_SIZE]; // Contains longitude, latitude, and angle

#define BUFFER_SIZE 2
float buf[BUFFER_SIZE]; // Contains only A1 and A2 data at a time

int main() {
    /****** DFT CALCULATION INIT ******/

    /* 
    * NB! due to errors in SPI driver on the DJI manifold
    * the UAS DFT calculations for the antenna output are
    * not included in code, but tested seperately. Instead
    * the data output for the transceiver search and coarse
    * search is emulated.
    */

    /****** UNIX DOMAIN SOCKET ******/

    int server_sock, len, rc;
    struct sockaddr_un server_adress;
    /* 
    * Clear the whole struct to avoid portability issues,
    * where some implementations have non-standard fields. 
    */
    memset(&server_adress, 0, sizeof(struct sockaddr_un));

    // Create a socket
    server_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        printf("SOCKET ERROR\n");
        exit(EXIT_FAILURE);
    }

    // Set up the sockaddr struct with the path
    server_adress.sun_family = AF_UNIX;
    strcpy(server_adress.sun_path, SERVER_PATH);
    memset(buf, 0, sizeof(float) * BUFFER_SIZE);

    int count = 0;
    int timeOutSet = 0;

    // Variables for antenna data generation
    double posLat, posLon, angle iX, iY;
    float A1, A2;
    int runOnce = 0;
    // The transceiver position is set X and Y distance from take-off
    int tX = 12;
    int tY = 93;

    while (1) {
        /****** START OF ANTENNA DATA GENERATION ******/

        // Receive data for data generation
        rc = recvfrom(server_sock, recvBuf, sizeof(float) * RECV_BUFFER_SIZE, 0, (struct sockaddr*)&server_adress,
                      &sizeof(server_adress));
        if (rc == -1) {
            if (timeOutSet == 0) {
                printf("RECEIVE ERROR\n");
                timeOutSet = 1;
            }
        } else {
            // Data is being received
            recvBuf[0] = posLon;
            recvBuf[1] = posLat;
            recvBuf[2] = angle;

            // Calculate position to receive
            if (runOnce == 0) {
                iY = calcMfromLat(posLat);
                iX = calcMfromLon(posLat, posLon);
                runOnce = 1;
            }

            // Updates the distance
            float dY = calcMfromLat(posLat) - iY;
            float dX = calcMfromLon(posLat, posLon) - iX;
            //calculates the distance between the UAV and the target
            float distanceTo = sqrt(pow((dX - tX), 2) + pow((dY - tY), 2));
            //Approximates the signal strength based on the distance
            int maxADCvalue = 4096;
            float signalStrength = maxADCvalue * (1 / pow(distanceTo, 3));
            //Finds the difference between the UAV's angle and the target's angle
            float targetAngle = 180 - 2 * getAngle(dY - tY, dX - tX);
            if (targetAngle < 0) {
                targetAngle += 360;
            }

            float diffAngle = targetAngle - angle;
            A1 = fabs(signalStrength * cos((diffAngle * M_PI / 180) + M_PI_4));
            A2 = fabs(signalStrength * cos((diffAngle * M_PI / 180) - M_PI_4));
            buf[0] = A1;
            buf[1] = A2;
        }

        /****** END OF ANTENNA DATA GENERATION ******/

        // Send the data to client
        rc = sendto(server_sock, buf, sizeof(float) * BUFFER_SIZE, 0, (struct sockaddr*)&server_adress,
                    sizeof(server_adress));
        if (rc == -1) {
            if (count == 0) {
                printf("SEND ERROR: NO SOCKET AVAILABLE. WAITING");
                fflush(stdout);
                timeOutSet = 1;
            } else if (count % 300 == 0) {
                printf(".");
                fflush(stdout);
                // If no clients are available after 60 s, stop data transfer and close process
            } else if (count > 6000) {
                count = 0;
                printf("\nNo client timing out...\n");
                close(server_sock);
                exit(EXIT_FAILURE);
            }
            usleep(10000); // 10 ms
            count++;
        } else {
            if (timeOutSet == 1) {
                printf("\nConnection reestablished. Sending data...\n");
                timeOutSet = 0;
            }
            // Data is being sent here!
        }
    }

    return 0;
}
