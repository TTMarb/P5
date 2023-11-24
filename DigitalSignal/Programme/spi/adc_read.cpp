#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/spi/spidev.h> // File that's located on the manifold in /usr/include/
#include <linux/types.h>
#include <sys/ioctl.h> // File that's located on the manifold in /usr/include/
#include <string>
#include <cstdint>
#include <thread>
#include <unistd.h>

using namespace std;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

const char* spi_device = "/dev/spidev3.0"; // SPI device file
int spi_fd;

void open_and_configure() {

    uint8_t spi_mode = SPI_MODE_0; // SPI mode (mode 0, 1, 2, or 3)
    uint8_t bpw = 8;               // Bits per word (8, 16, etc.)
    uint32_t speed = 32000000;      // SPI speed (Hz)

    spi_fd = open(
        spi_device,
        O_RDWR); //Opens the the file "spi_device" in function O_RDWR "Read or write". Save file descriptor in spi_fd
    if (spi_fd < 0) {
        cerr << "Failed to open SPI device." << endl;
        return;
    }

    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) == -1 || ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bpw) == -1
        || ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        perror("Error setting SPI parameters");
        close(spi_fd);
    }
}

int main() {
    open_and_configure();
    uint8_t data[256];
    uint8_t buffer[1];

    struct spi_ioc_transfer tr; 
    tr.tx_buf = 0; // We are not sending any data
    tr.rx_buf = (unsigned long)buffer; // Buffer to store received data
    tr.len = ARRAY_SIZE(buffer); // Number of bytes to read

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("SPI transfer failed");
        return -1; // Error handling
    }


    return 0;
}