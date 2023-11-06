#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>



#include <iostream>
#include "spi.h"

#define SPI_BASEADDR 0x43d00000



void spiMode(int a){
volatile uint32_t* BIT_LEN=reinterpret_cast<volatile uint32_t*>(SPI_BASEADDR);
*BIT_LEN |=(1<<4);

}