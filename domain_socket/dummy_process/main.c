
/************************************************************/
/* This is a stream socket client sample program for UNIX   */
/* domain sockets. This program creates a socket, connects  */
/* to a server, sends data, then receives and prints a      */
/* message from the server.                                 */
/************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#define SERVER_PATH "/tmp/unix_sock.server"
//#define SERVER_PATH "tpf_unix_sock.server"
#define CLIENT_PATH "tpf_unix_sock.client"
#define DATA        "Hello from client"

int main(void) {

    /* Create a child process for the antenna DFT process */
    pid_t pid;

    /*
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    */
    pid = fork();

    if (pid == -1) {
        printf("Error while forking antenna_dft process\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("DFT child process initiated. PID is: %d\n", getpid());
        char* const args[] = {"tmp", NULL};
        char* const envp[] = {NULL};

        execve("/tmp/antenna_dft.bin", args, envp);
        perror("execve");
        exit(EXIT_FAILURE); /* Exit the child process if it fails*/
    } else if (pid > 0) {
        printf("Transceiver parent process running. PID is: %d\n", getpid());
        printf("In else if statement\n");
        //exit(EXIT_SUCCESS);
        printf("After if else statement\n");
    }

    printf("Continue after spawning child\n");
    exit(EXIT_SUCCESS);

    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        printf("SOCKET ERROR\n");
        exit(1);
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = sizeof(client_sockaddr);

    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr*)&client_sockaddr, len);
    if (rc == -1) {
        printf("BIND ERROR\n");
        close(client_sock);
        exit(1);
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* for the server socket and connect   */
    /* to it.                              */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr*)&server_sockaddr, len);
    if (rc == -1) {
        printf("CONNECT ERROR\n");
        close(client_sock);
        exit(1);
    }

    /************************************/
    /* Copy the data to the buffer and  */
    /* send it to the server socket.    */
    /************************************/
    strcpy(buf, DATA);
    printf("Sending data...\n");
    rc = send(client_sock, buf, strlen(buf), 0);
    if (rc == -1) {
        printf("SEND ERROR\n");
        close(client_sock);
        exit(1);
    } else {
        printf("Data sent!\n");
    }

    /**************************************/
    /* Read the data sent from the server */
    /* and print it.                      */
    /**************************************/
    printf("Waiting to recieve data...\n");
    memset(buf, 0, sizeof(buf));
    rc = recv(client_sock, buf, sizeof(buf), 0);
    if (rc == -1) {
        printf("RECV ERROR\n");
        close(client_sock);
        exit(1);
    } else {
        printf("DATA RECEIVED = %s\n", buf);
    }

    /******************************/
    /* Close the socket and exit. */
    /******************************/
    close(client_sock);

    return 0;
}