#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/unix_sock.server"
#define BUFFER_SIZE 10
float buf[BUFFER_SIZE];

int main(void) {

    int client_sock, rc, len;
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
            int j;
            for (j = 0; j < BUFFER_SIZE; j++) {
                if (buf[i] >= 15) { // NB! edit to desired threshold
                    close(client_sock)
                        stopMission(vehicle, responseTimeout, 0); // Stop waypoint mission if threshold is reached
                    printf("Stopping waypoint mission...\n");
                    exit(EXIT_SUCCESS); // Exit process
                }
            }
        }
    }
    return 0;
}