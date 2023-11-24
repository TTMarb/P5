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

    // Set up the two 16-bit messages
    uint16_t message1 = 0;
    uint16_t message2 = 0;

    // Prepare the SPI transfer structure
    struct spi_ioc_transfer tr[2];

    tr[0].tx_buf = 0;  // We're not sending in this example
    tr[0].rx_buf = (unsigned long)&message1;
    tr[0].len = sizeof(message1);
    tr[0].cs_change = 1;

    tr[1].tx_buf = 0;  // We're not sending in this example
    tr[1].rx_buf = (unsigned long)&message2;
    tr[1].len = sizeof(message2);
    tr[1].cs_change = 1;

    // Perform SPI transfer
    if (ioctl(spi_fd, SPI_IOC_MESSAGE(2), &tr) < 0) {
        perror("Error during SPI transfer");
        close(spi_fd);
        return 1;
    }

    // Close SPI device
    close(spi_fd);

    return 0;
}