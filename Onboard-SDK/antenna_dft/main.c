/* Process for reading antenna outputs and performing DFT.
 * The calculations are sent using a UNIX domain socket.
 *  
 */

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include "antenna.h"
#define searchRadius     10

/* Path for UNIX domain socket */
#define SERVER_PATH      "/tmp/unix_sock.server"

// Buffer for sending the gps info for the data generator in antenna_dft
#define RECV_BUFFER_SIZE 3
double recvBuf[RECV_BUFFER_SIZE]; // Contains longitude, latitude, and angle

#define BUFFER_SIZE 2
float buf[BUFFER_SIZE]; // Contains only A1 and A2 data at a time

int main() {

    /****** UNIX DOMAIN SOCKET ******/

    int server_sock, len, rc;
    struct sockaddr_un server_adress, peer_sock;
    /* 
    * Clear the whole struct to avoid portability issues,
    * where some implementations have non-standard fields. 
    */
    memset(&server_adress, 0, sizeof(struct sockaddr_un));
    memset(&peer_sock, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, sizeof(float) * BUFFER_SIZE);
    memset(recvBuf, 0, sizeof(float) * RECV_BUFFER_SIZE);

    // Create a socket
    server_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        printf("SOCKET ERROR\n");
        exit(EXIT_FAILURE);
    }

    // Set up the sockaddr struct with the path
    server_adress.sun_family = AF_UNIX;
    strcpy(server_adress.sun_path, SERVER_PATH);

    len = sizeof(server_adress);

    unlink(SERVER_PATH);
    rc = bind(server_sock, (struct sockaddr*)&server_adress, len);
    if (rc == -1) {
        printf("BIND ERROR\n");
        close(server_adress);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    int timeOutSet = 0;

    /*
    // Set the socket to non-blocking when receiving data
    int flags = fcntl(server_sock, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(server_sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    */
    FILE* file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
    }
    fprintf(file, "OUTPUT\n");
    fclose(file);

    // Variables for antenna data generation
    double posLat, posLon, angle, iX, iY;
    float A1, A2;
    int runOnce = 0;
    int calComplete = 0;
    // The transceiver position is set X and Y distance from take-off
    int tX = 12;
    int tY = 93;
    while (1) {
        /****** START OF ANTENNA DATA GENERATION ******/

        /* 
        * NB! due to errors in SPI driver on the DJI manifold
        * the UAS DFT calculations for the antenna output are
        * not included in code, but tested seperately. Instead
        * the data output for the transceiver search and coarse
        * search is emulated.
        */

        // Receive data for data generation
        FILE* file = fopen("output.txt", "a");
        if (file == NULL) {
            printf("Error opening file\n");
        }
        fprintf(file, "Am here\n");
        fclose(file);

        // Keep in a blocked state until receiving data
        rc = recv(server_sock, recvBuf, sizeof(float) * RECV_BUFFER_SIZE, 0);
        if (rc == -1) {
            //printf("ANTENNA RECEIVE ERROR\n");
            perror("recvfrom");
        } else {
            // Data is being received
            FILE* file = fopen("output.txt", "a");
            if (file == NULL) {
                printf("Error opening file\n");
            }
            fprintf(file, "Receiving buffer %f, %f, %f\n", recvBuf[0], recvBuf[1], recvBuf[2]);
            fclose(file);
            posLon = recvBuf[0];
            posLat = recvBuf[1];
            angle = recvBuf[2];

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

            calComplete = 1;

            /****** END OF ANTENNA DATA GENERATION ******/
        }

        // Send the data to client after completion of calculation
        if (calComplete == 1) {
            rc = send(server_sock, buf, sizeof(float) * BUFFER_SIZE, 0);
            if (rc == -1) {
                if (count == 0) {
                    printf("SEND ERROR: NO SOCKET AVAILABLE. WAITING");
                    fflush(stdout);
                    timeOutSet = 1;
                } else if (count % 300 == 0) {
                    printf(".");
                    fflush(stdout);
                    // If no clients are available after 2 min, stop data transfer and close process
                } else if (count > 12000) {
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
                FILE* file = fopen("output.txt", "a");
                if (file == NULL) {
                    printf("Error opening file\n");
                }
                fprintf(file, "Sending data %f, %f\n", buf[0], buf[1]);

                fclose(file);

                calComplete = 0;
            }
        }
    }

    return 0;
}
