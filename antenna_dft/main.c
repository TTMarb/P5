
#include <stdint.h>

uint8_t runOnce = 0;

int main {
    if (runOnce == 0) {
        printf("Started antenna_dft");
        runOnce = 1;
    }
}