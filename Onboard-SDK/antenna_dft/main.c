/* Process for reading antenna outputs and performing DFT.
 * The calculations are sent using a UNIX domain socket.
 *  
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

/* Path for UNIX domain socket */
#define SERVER_PATH "/tmp/unix_sock.server"
#define BUFFER_SIZE 10
float buf[BUFFER_SIZE];

int main() {
    /****** DFT CALCULATIONS ******/

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

    float top = 10; // Define threshold for number generator
    int count = 0;
    int timeOutSet = 0;

    printf("Sending data...\n");
    while (1) {
        // Generate random values below, placeholder for dft data
        int i;
        for (i = 0; i < BUFFER_SIZE; i++) {
            float numGen = ((float)rand() / (float)(RAND_MAX)) * top;
            buf[i] = numGen;
        }

        // Send the data to server
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
