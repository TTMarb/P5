#ifndef SOCK_H
#define SOCK_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>

//socket related includes
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>


// Path for UNIX domain socket
#define SERVER_PATH      "/tmp/unix_sock.server"
#define CLIENT_PATH      "/tmp/unix_sock.client"

// Buffer for sending the gps info for the data generator in antenna_dft
#define SEND_BUFFER_SIZE 3

// Buffer for receiving antenna data
#define BUFFER_SIZE 2

class sock {
  public:
    int client_sock, rc;
    sock(); // Constructor
    bool send(double,double,double);
    bool receive(float*, float*);
  private:
    double sendBuf[SEND_BUFFER_SIZE]; // Contains longitude, latitude, and angle
    float buf[BUFFER_SIZE]; // Contains only A1 and A2 data at a time
    int timeOutSet;
    uint32_t len;
    struct sockaddr_un client_sockaddr, server_sockaddr;
};

#endif // SOCK_H
