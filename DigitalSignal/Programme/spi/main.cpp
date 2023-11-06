#include <iostream>
#include <stdio.h>

using namespace std;

#define SPI_BASEADDR 0x43d00000

int main(){

#define SPI_COMMAND1_BIT_LEN 0x04 //define the length of the message -1
*(volatile uint32_t *)SPI_COMMAND1_BIT_LEN = 14;

#define SPI_DMA_CTL_IERX 0x29 // enable interrup on the receiver buffer
*(volatile uint32_t *)SPI_DMA_CTL_IERX = 1;


#define SPI_DMA_CTL_DMA 0x31 //enable DMA, must be set last
*(volatile uint32_t *)SPI_DMA_CTL_DMA = 1;

volatile uint32_t* BIT_LEN=reinterpret_cast<volatile uint32_t*>(SPI_BASEADDR);
*BIT_LEN |=(1<<4);

cout <<SPI_BASEADDR<<endl;
}