#include <iostream>
#include <string>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    // Define the SPI device file and other configuration settings
    std::string device = "/dev/spidev0.0"; // SPI device file
    int spi_fd;
    uint8_t spi_mode = SPI_MODE_0; // SPI mode (mode 0, 1, 2, or 3)
    uint8_t bits_per_word = 8;    // Bits per word (8, 16, etc.)
    uint32_t speed = 500000;      // SPI speed (Hz)

    // Open the SPI device
    spi_fd = open(device.c_str(), O_RDWR);
    if (spi_fd < 0) {
        std::cerr << "Failed to open SPI device." << std::endl;
        return 1;
    }

    // Configure SPI mode
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) == -1) {
        std::cerr << "Failed to set SPI mode." << std::endl;
        return 1;
    }

    // Configure bits per word
    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) == -1) {
        std::cerr << "Failed to set bits per word." << std::endl;
        return 1;
    }

    // Configure SPI speed
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        std::cerr << "Failed to set SPI speed." << std::endl;
        return 1;
    }

    // Data to be transmitted (example data)
    uint8_t tx_data[] = {0x01, 0x02, 0x03};

    // Buffer for received data
    uint8_t rx_data[sizeof(tx_data)];

    // Transfer data over SPI
    struct spi_ioc_transfer tr = {};
    tr.tx_buf = (unsigned long)tx_data;
    tr.rx_buf = (unsigned long)rx_data;
    tr.len = sizeof(tx_data);

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        std::cerr << "SPI transfer error." << std::endl;
        return 1;
    }

    // Print received data (example)
    std::cout << "Received data:";
    for (size_t i = 0; i < sizeof(rx_data); i++) {
        std::cout << " " << std::hex << static_cast<int>(rx_data[i]);
    }
    std::cout << std::endl;

    // Close the SPI connection
    close(spi_fd);

    return 0;
}
