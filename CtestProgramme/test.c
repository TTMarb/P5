#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(){
printf("\nA sample C program\n\n");

int serial_port = open( "/dev/ttyTHS0", O_RDWR| O_NOCTTY );


if (serial_port < 0){
	printf("Error %i from open %s\n", errno, strerror(errno));
}


struct termios tty;

if(tcgetattr(serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
}

//Inspireret af https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

/* Set Baud Rate */
cfsetspeed(&tty, B19200);
tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
tty.c_cflag |= CS8; // 8 bits per byte (most common)
tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

tty.c_lflag &= ~ECHO; // Disable echo
tty.c_lflag &= ~ECHOE; // Disable erasure
tty.c_lflag &= ~ECHONL; // Disable new-line echo

tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
}

printf("About to write 'Bonjour World'");
unsigned char msg[] = { 'B', 'o', 'n', 'j', 'o','u','r', ' ', 'w', 'o', 'r','l','d','\r','\n'};
write(serial_port, msg, sizeof(msg));
printf("About to write 'Gutentag Welt'");
unsigned char msg1[] = { 'G','u','t','e','n','t','a','g',' ', 'w', 'e','l','t','\r','\n'};
write(serial_port, msg1, sizeof(msg1));




return 0;
}
