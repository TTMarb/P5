#include "sock.h"

//Constructor
sock::sock() {
    /********* START OF DOMAIN SOCKET *********/
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

}

bool sock::send(std::string string) {
}

bool sock::receive(){
    int rc = recv(client_sock, buf, sizeof(float) * BUFFER_SIZE, 0);
        if (rc == -1) {
            if (timeOutSet == 0) {
                perror("recv");
                timeOutSet = 1;
                return false;
            }
        }else{
            // Error message if connection was briefly lost
            if (timeOutSet == 1) {
                printf("\nConnection restablished. Receiving data...\n");
                timeOutSet = 0;
            }
            return true;
        }
}