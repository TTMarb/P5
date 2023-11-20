/*
 * UNIX domain socket for transmitting antenna data 
 * during the coarse search stage. The code is the 
 * for the DFT process (server) to initiate a connection-
 * oriented data stream for the coarse search process
 * (client).
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define SOCK_PATH "tpf_unix_sock.server"
#define DATA      "Hello from server"

int main() {
    int server_sock, client_sock, len, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    int backlog = 10;

    /* Clear the whole struct to avoid portability issues,
 * where some implementations have non-standard fields. 
 */
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);

    /* Create a socket */
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1) {
        printf("SOCKET ERROR\n");
        exit(1);
    }

    /* Bind socket to the socket name */
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCK_PATH);
    len = sizeof(server_sockaddr);

    /* Unlink file before binding it */
    unlink(SOCK_PATH);
    rc = bind(server_sock, (struct sockaddr*)&server_sockaddr, len);
    if (rc == -1) {
        printf("BIND ERROR\n");
        close(server_sock);
        exit(1);
    }

    /* Listen for client sockets */
    rc = listen(server_sock, backlog);
    if (rc == -1) {
        printf("LISTEN ERROR\n");
        close(server_sock);
        exit(1);
    }
    printf("socket listening...\n");

    for (;;) {
        /* Accept incoming communication */
        client_sock = accept(server_sock, (struct sockaddr*)&client_sockaddr, &len);
        if (client_sock == -1) {
            printf("ACCEPT ERROR\n");
            close(server_sock);
            close(client_sock);
            exit(1);
        }

        /* Display socket name */
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
        /* Print incoming data */
        printf("waiting to read...\n");
        bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
        if (bytes_rec == -1) {
            printf("RECV ERROR\n");
            close(server_sock);
            close(client_sock);
            exit(1);
        } else {
            printf("DATA RECEIVED = %s\n", buf);
        }
    }

    /*

    // Resend data to connected socket 
    memset(buf, 0, 256);
    strcpy(buf, DATA);
    printf("Sending data...\n");
    rc = send(client_sock, buf, strlen(buf), 0);
    if (rc == -1) {
        printf("SEND ERROR\n");
        close(server_sock);
        close(client_sock);
        exit(1);
    } else {
        printf("Data sent!\n");
    }
    * /

        /* Close the socket connection and exit */
    close(server_sock);
    close(client_sock);

    return 0;
}
