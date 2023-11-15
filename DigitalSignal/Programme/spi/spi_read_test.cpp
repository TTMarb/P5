#include <fcntl.h>
#include <iostream>
#include <cstring>
//#include <linux/spi/spidev.h> // File that's located on the manifold in /usr/include/
#include <string>
//#include <sys/ioctl.h> // File that's located on the manifold in /usr/include/
#include <unistd.h>
#include <cstdint>

using namespace std;

const char *spiDevice = "/dev/spidev3.0"; // SPI device file
int spi_fd;

//Opens the SPI device and configures the mode, bits per word and speed.
void open_and_configure() {

    uint8_t spi_mode = SPI_MODE_0; // SPI mode (mode 0, 1, 2, or 3)
    uint8_t bits_per_word = 8;     // Bits per word (8, 16, etc.)
    uint32_t speed = 1000000;     // SPI speed (Hz)

    // Open the SPI device
    spi_fd = open(spiDevice, O_RDWR);
    if (spi_fd < 0) {
        cerr << "Failed to open SPI device." << endl;
        return;
    }

    // Configure SPI mode
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) == -1) {
        cerr << "Failed to set SPI mode." << endl;
        return;
    }

    // Configure bits per word
    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) == -1) {
        cerr << "Failed to set bits per word." << endl;
        return;
    }

    // Configure SPI speed
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        cerr << "Failed to set SPI speed." << endl;
        return;
    }
}

int main(){
    
    char buffer[256];

    while (true) {
        // Read data from SPI
        int bytesRead = read(spi_fd, buffer, sizeof(buffer));

        if (bytesRead < 0) {
            perror("Error reading from SPI");
            break;
        }

        // Process the received data (you can modify this part according to your requirements)
        if (bytesRead > 0) {
            std::cout << "Received " << bytesRead << " bytes: ";
            for (int i = 0; i < bytesRead; ++i) {
                std::cout << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::endl;
        }

        // Add a delay or other logic as needed
        usleep(100000);  // Sleep for 100 milliseconds (adjust as needed)
    }
    return 0;
}