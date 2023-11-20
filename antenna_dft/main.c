#include "stdint.h"
#include "stdio.h"

uint8_t runOnce = 0;

int main(void) {
    if (runOnce == 0) {
        printf("Started antenna_dft\n");
        runOnce = 1;
    }
}