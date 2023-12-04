#include <iostream>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    int spi_fd;
    const char *device = "/dev/spidev0.0";  // Use the appropriate SPI device file
    uint8_t mode = SPI_MODE_0;
    uint8_t bits_per_word = 8;
    uint32_t speed = 500000;  // Set your desired speed in Hz

    // Open the SPI device
    spi_fd = open(device, O_RDWR);
    if (spi_fd < 0) {
        std::cerr << "Failed to open SPI device." << std::endl;
        return 1;
    }

    // Set SPI mode
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) == -1) {
        std::cerr << "Failed to set SPI mode." << std::endl;
        return 1;
    }

    // Set bits per word
    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) == -1) {
        std::cerr << "Failed to set bits per word." << std::endl;
        return 1;
    }

    // Set SPI speed
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        std::cerr << "Failed to set SPI speed." << std::endl;
        return 1;
    }

    // Transfer data and process as before

    // Close the SPI device
    close(spi_fd);

    return 0;
}
