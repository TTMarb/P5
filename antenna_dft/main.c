#include "stdint.h"
#include "stdio.h"

int main(void) {
    int i;
    for (i = 0; i < 2000; i++) {
        if (i == 0) {
            printf("Began count!\n");
        }
        usleep(5000);
        }
    printf("Ended count\n");
}