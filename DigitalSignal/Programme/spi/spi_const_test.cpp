#include <iostream>
#include <string>
#include <fcntl.h>
#include <linux/spi/spidev.h> // File that's located on the manifol in /usr/include/
#include <sys/ioctl.h> // File that's located on the manifol in /usr/include/
#include <unistd.h>

using namespace std;

//Opens the SPI device and configures the mode, bits per word and speed.
void open_and_configure(int *spi_fd, uint8_t spi_mode, uint8_t bits_per_word, uint32_t speed){
    // Open the SPI device

    *spi_fd = open(device.c_str(), O_RDWR);
    if (*spi_fd < 0) {
        cerr << "Failed to open SPI device." << endl;
        return;
    }

    // Configure SPI mode
    if (ioctl(*spi_fd, SPI_IOC_WR_MODE, &spi_mode) == -1) {
        cerr << "Failed to set SPI mode." << endl;
        return;
    }

    // Configure bits per word
    if (ioctl(*spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) == -1) {
        cerr << "Failed to set bits per word." << endl;
        return;
    }

    // Configure SPI speed
    if (ioctl(*spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        cerr << "Failed to set SPI speed." << endl;
        return;
    }
}

int main() {
    // Define the SPI device file and other configuration settings
    string device = "/dev/spidev0.0"; // SPI device file
    int spi_fd;
    uint8_t spi_mode = SPI_MODE_0; // SPI mode (mode 0, 1, 2, or 3)
    uint8_t bits_per_word = 8;    // Bits per word (8, 16, etc.)
    uint32_t speed = 50000000;      // SPI speed (Hz)

    open_and_configure(&spi_fd, spi_mode, bits_per_word, speed);

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
        cerr << "SPI transfer error." << endl;
        return 1;
    }

    // Print received data (example)
    cout << "Received data:";
    for (size_t i = 0; i < sizeof(rx_data); i++) {
        cout << " " << hex << static_cast<int>(rx_data[i]);
    }
    cout << endl;

    // Close the SPI connection
    close(spi_fd);

    return 0;
}

