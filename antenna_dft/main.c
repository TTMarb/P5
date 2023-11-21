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

    /****** UNIX DOMAIN SOCKET ******/

    int server_sock, len, rc;
    int bytes_rec = 0;
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

    // Generate data to send
    memset(buf, 0, sizeof(float) * BUFFER_SIZE);
    printf("Sending data...\n");
    int i;
    float num = 5;
    for (i = 0; i < 5; i++) {
        num = num + 0.1;
        buf[i] = num;
        printf("%f\n", buf[i]);
    }

    // Send data to clients
    rc = sendto(server_sock, buf, sizeof(float) * BUFFER_SIZE, 0, (struct sockaddr*)&server_adress,
                sizeof(server_adress));
    if (rc == -1) {
        printf("SEND ERROR\n");
        close(server_sock);
        exit(EXIT_FAILURE);
    } else {
        printf("Data sent!\n");
    }

    /* 
    // Listen for client sockets
    rc = listen(server_sock, backlog);
    if (rc == -1) {
        printf("LISTEN ERROR\n");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    printf("socket listening...\n");

    // Accept incoming communication
    client_sock = accept(server_sock, (struct sockaddr*)&client_sockaddr, &len);
    if (client_sock == -1) {
        printf("ACCEPT ERROR\n");
        close(server_sock);
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Display socket name
    len = sizeof(client_sockaddr);
    rc = getpeername(client_sock, (struct sockaddr*)&client_sockaddr, &len);
    if (rc == -1) {
        printf("GETPEERNAME ERROR\n");
        close(server_sock);
        close(client_sock);
        exit(EXIT_FAILURE);
    } else {
        printf("Client socket filepath: %s\n", client_sockaddr.sun_path);
    }

    // Send data to connected socket
    memset(buf, 0, sizeof(float) * BUFFER_SIZE);
    int i;
    float num = 6;
    for (i = 0; i < 5; i++) {
        num = num + 0.1;
        buf[i] = num;
    }
    printf("Sending data...\n");

    for (i = 0; i < BUFFER_SIZE; i++) {
        printf("%f\n", buf[i]);
    }

    rc = send(client_sock, buf, sizeof(float) * BUFFER_SIZE, 0);
    if (rc == -1) {
        printf("SEND ERROR\n");
        close(server_sock);
        close(client_sock);
        exit(EXIT_FAILURE);
    } else {
        printf("Data sent!\n");
    }

     */

    // Close the socket connection and exit
    close(server_sock);
    //close(client_sock);

    return 0;
}
