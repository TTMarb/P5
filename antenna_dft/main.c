/* Process for reading antenna outputs and performing DFT.
 * The calculation are sent using 
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
#define SOCK_PATH   "/tmp/unix_sock.server"
#define BUFFER_SIZE 10
float buf[BUFFER_SIZE];

int main() {
    /****** DFT CALCULATIONS ******/

    /****** UNIX DOMAIN SOCKET ******/

    int server_sock, client_sock, len, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    int backlog = 10;
    /* 
    * Clear the whole struct to avoid portability issues,
    * where some implementations have non-standard fields. 
    */
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, BUFFER_SIZE);

    // Create a socket
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1) {
        printf("SOCKET ERROR\n");
        exit(1);
    }

    // Set up the sockaddr struct with the path
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCK_PATH);
    len = sizeof(server_sockaddr);

    // Unlink before bind to ensure a correct bind
    unlink(SOCK_PATH);

    // Bind socket to the socket name
    rc = bind(server_sock, (struct sockaddr*)&server_sockaddr, len);
    if (rc == -1) {
        printf("BIND ERROR\n");
        close(server_sock);
        exit(1);
    }

    // Listen for client sockets
    rc = listen(server_sock, backlog);
    if (rc == -1) {
        printf("LISTEN ERROR\n");
        close(server_sock);
        exit(1);
    }
    printf("socket listening...\n");

    // Accept incoming communication
    client_sock = accept(server_sock, (struct sockaddr*)&client_sockaddr, &len);
    if (client_sock == -1) {
        printf("ACCEPT ERROR\n");
        close(server_sock);
        close(client_sock);
        exit(1);
    }

    // Display socket name
    len = sizeof(client_sockaddr);
    rc = getpeername(client_sock, (struct sockaddr*)&client_sockaddr, &len);
    if (rc == -1) {
        printf("GETPEERNAME ERROR\n");
        close(server_sock);
        close(client_sock);
        exit(1);
    } else {
        printf("Client socket filepath: %s\n", client_sockaddr.sun_path);
    }

    // Send data to connected socket
    memset(buf, 0, BUFFER_SIZE);

    int i;
    for (i = 0; i < 5; i++) {
        float num = 5;
        num = num * ((float)rand() / (float)(RAND_MAX)); // Generate random values between 0 and 5
        buf[i] = num;
    }
    printf("Sending data...\n");
    rc = send(client_sock, buf, BUFFER_SIZE, 0);
    if (rc == -1) {
        printf("SEND ERROR\n");
        close(server_sock);
        close(client_sock);
        exit(1);
    } else {
        printf("Data sent!\n");
    }

    // Close the socket connection and exit
    close(server_sock);
    close(client_sock);

    return 0;
}
