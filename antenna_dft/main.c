#include "stdint.h"
#include "stdio.h"

int main(void) {
    for (int i = 0; i < 30; i++) {
        if (i == 0) {
            printf("Began count!\n");
        }
        printf("The count is %d", i);
        sleep(50);
    }
}