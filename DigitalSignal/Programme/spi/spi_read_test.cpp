#include <fcntl.h>
#include <iostream>
#include <chrono>
#include <cstring>
#include <linux/spi/spidev.h> // File that's located on the manifold in /usr/include/
#include <string>
#include <sys/ioctl.h> // File that's located on the manifold in /usr/include/
#include <unistd.h>
#include <cstdint>
#include <thread>

using namespace std;

const char *spi_device = "/dev/spidev3.0"; // SPI device file
int spi_fd;

//Opens the SPI device and configures the mode, bits per word and speed.
void open_and_configure() {

    uint8_t spi_mode = SPI_MODE_0; // SPI mode (mode 0, 1, 2, or 3)
    uint8_t bpw = 8;     // Bits per word (8, 16, etc.)
    uint32_t speed = 50000000;     // SPI speed (Hz)

    spi_fd = open(spi_device, O_RDWR); //Opens the the file "spi_device" in function O_RDWR "Read or write". Save file descriptor in spi_fd
    if (spi_fd < 0) {
        cerr << "Failed to open SPI device." << endl;
        return;
    }

    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) == -1 ||
        ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bpw) == -1 ||
        ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) 
    {
        perror("Error setting SPI parameters");
        close(spi_fd);
    }

}

void continuousSpiRead(int spi_fd, size_t bufferSize) {
    // Allocate a buffer for reading data
    unsigned char* buffer = new unsigned char[bufferSize];

    // Set up a spi_ioc_transfer structure
    struct spi_ioc_transfer transfer = {
        .tx_buf = 0,  // No data to transmit
        .rx_buf = (unsigned long)buffer,
        .len = bufferSize,
        .delay_usecs = 0,
        .speed_hz = 1000000,  // Set the desired speed in Hz
        .bits_per_word = 8,
    };

    // Infinite loop for continuous reading
    while (true) {
        // Read data from SPI device using ioctl
        auto start_time = chrono::high_resolution_clock::now();
        if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &transfer) == -1) {
            perror("Error reading from SPI");
            break;
        }
        auto end_time = chrono::high_resolution_clock::now();

        // Process the read data (you may want to replace this with your logic)
        // For example, print the received bytes:
        std::cout << "Received " << transfer.len << " bytes: ";
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        cout << duration.count() << " microseconds: ";
        for (size_t i = 0; i < transfer.len; ++i) {
            std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
        }
        std::cout << std::endl;

        // Add any necessary delay or sleep here if needed
        //usleep(100);  // Sleep for 100 milliseconds (adjust as needed)
    }

    // Cleanup allocated memory
    delete[] buffer;
}

void readSPI(int spi_fd) {
    char buffer[256];
    
    while (true) {
        auto start_time = chrono::high_resolution_clock::now();

        int bytes_read = read(spi_fd, buffer, sizeof(buffer));
        
        auto end_time = chrono::high_resolution_clock::now();

        if (bytes_read < 0) {
            perror("Error reading from SPI");
            break;
        }

        if (bytes_read > 0) {
            cout << "Received " << bytes_read << " bytes: ";
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            cout << duration.count() << " microseconds: ";
            for (int i = 0; i < bytes_read; ++i) {
                cout << static_cast<int>(buffer[i]) << " ";
            }
            cout << endl;
        }
    }
}

int main(){
    open_and_configure();
    continuousSpiRead(spi_fd,256);
    //readSPI(spi_fd);
    return 0;
}