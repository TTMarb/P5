#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/unix_sock.server"
//#define CLIENT_PATH "tpf_unix_sock.client"
#define BUFFER_SIZE 10
float buf[BUFFER_SIZE];

int main(void) {

    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    /* 
    * Clear the whole struct to avoid portability issues,
    * where some implementations have non-standard fields. 
    */
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    // Create a socket
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        printf("SOCKET ERROR\n");
        exit(1);
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
        exit(1);
    }
    */

    // Set up the sockaddr struct for the server and connect
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr*)&server_sockaddr, len);
    if (rc == -1) {
        printf("CONNECT ERROR\n");
        close(client_sock);
        exit(1);
    }

    // Read and print data from server
    printf("Waiting to recieve data...\n");
    memset(buf, 0, BUFFER_SIZE);
    rc = recv(client_sock, buf, BUFFER_SIZE, 0);
    if (rc == -1) {
        printf("RECV ERROR\n");
        close(client_sock);
        exit(1);
    } else {
        int i;
        for (i = 0; i < BUFFER_SIZE; i++) {
            printf("Buffer contains\n");
            printf("%lf\n", buf[i]);
        }
    }

    // Close socket and exit
    close(client_sock);

    return 0;
}