#include <iostream>
#include <cstdint>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <bitset>



// Define the base address of the register you want to read (replace with actual address)
#define REGISTER_BASE_ADDRESS 0x7000e000

int main() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        std::cerr << "Failed to open /dev/mem" << std::endl;
        return 1;
    }

    // Map the register to a memory address
    uint32_t* register_address = (uint32_t*)mmap(NULL, sizeof(uint32_t), PROT_READ, MAP_SHARED, fd, REGISTER_BASE_ADDRESS);
    if (register_address == MAP_FAILED) {
        std::cerr << "Memory mapping failed" << std::endl;
        close(fd);
        return 1;
    }

    // Read and print the contents of the register
    uint32_t register_value = *register_address;
    std::cout << "Register Value: 0b" << std::bitset<32>(register_value) << std::endl;

    // Unmap the memory
    munmap(register_address, sizeof(uint32_t));
    close(fd);

    ioctl()


    return 0;
}
