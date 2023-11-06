#include <iostream>
#include <cstdint>
#include <sys/mman.h>
#include <fcntl.h>

// Define the base address of the register you want to read (replace with actual address)
#define REGISTER_BASE_ADDRESS 0x43d00000

int main() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        std::cerr << "Failed to open /dev/mem" << std::endl;
        return 1;
    }

    // Map the register to a memory address
    volatile uint32_t* register_address = (volatile uint32_t*)mmap(NULL, sizeof(uint32_t), PROT_READ, MAP_SHARED, fd, REGISTER_BASE_ADDRESS);
    if (register_address == MAP_FAILED) {
        std::cerr << "Memory mapping failed" << std::endl;
        close(fd);
        return 1;
    }

    // Read and print the contents of the register
    uint32_t register_value = *register_address;
    std::cout << "Register Value: 0x" << std::hex << register_value << std::dec << std::endl;

    // Unmap the memory
    munmap(register_address, sizeof(uint32_t));
    close(fd);

    return 0;
}
