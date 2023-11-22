#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/unix_sock.server"
//#define CLIENT_PATH "/tmp/transceiver_unix_sock.client"
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
        rc = recvfrom(client_sock, buf, sizeof(float) * BUFFER_SIZE, 0, (struct sockaddr*)&server_adress, &len);
        printf("RC val: %d\n", rc);
        if (rc == -1) {
            if (count == 0) {
                printf("RECEIVE ERROR: NO SERVER AVAILABLE. WAITING");
                fflush(stdout); // Flush the serial buffer without \n
                timeOutSet = 1;
            } else if (count % 600 == 0) {
                printf(".");
                fflush(stdout);
            } else if (count > 6000) { // 60 s
                count = 0;
                printf("\nNo server timing out...\n");
                exit(client_sock);
                exit(EXIT_FAILURE);
            }
            usleep(10000);
            count++;
        } else {
            if (timeOutSet == 1) {
                printf("\nConnection restablished. Receiving data...\n");
            }
            int i;
            for (i = 0; i < BUFFER_SIZE; i++) {
                printf("%f\n", buf[i]);
            }
        }
    }

    /* 
    // Set up the sockaddr struct for the client
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = sizeof(client_sockaddr);
    // Unlink before bind to ensure a correct bind
    unlink(CLIENT_PATH);
    // Bind socket to the socket name
    rc = bind(client_sock, (struct sockaddr*)&client_sockaddr, len);
    if (rc == -1) {
        printf("BIND ERROR\n");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Set up the sockaddr struct for the server and connect
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr*)&server_sockaddr, len);
    if (rc == -1) {
        printf("CONNECT ERROR\n");
        close(client_sock);
        exit(EXIT_FAILURE);
    }
 
    // Read and print data from server
    printf("Waiting to recieve data...\n");
    memset(buf, 0, sizeof(float) * BUFFER_SIZE);
    rc = recv(client_sock, buf, sizeof(float) * BUFFER_SIZE, 0);
    if (rc == -1) {
        printf("RECV ERROR\n");
        close(client_sock);
        exit(EXIT_FAILURE);
    } else {
        printf("Buffer contains\n");
        int i;
        for (i = 0; i < BUFFER_SIZE; i++) {
            printf("%f\n", buf[i]);
        }
    }

  */
    // Close socket and exit
    //close(client_sock);

    return 0;
}