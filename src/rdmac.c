////////////////////////////////////////////////////////////
//  rdmac.c
//  read MAC address of KSZ9896 switch & write it to console
//  b.r.koball
//  k.i.fire
//  V2.0.1
//  2018-11-07
//
// compile with: gcc rdmac.c reg.c -lwiringPi 
//
////////////////////////////////////////////////////////////

#include "reg.h"
#include <stdio.h>

//
// Command strings to read/write 
// Management Inferface MAC registers
// on KSZ9896 switch
//
//
uchar MAC_reg_addr[6][5] = { 
{0x00, 0x00, 0x60, 0x40, 0x00}, // MAC addr reg 0 (bits 47:40)
{0x00, 0x00, 0x60, 0x60, 0x00}, // MAC addr reg 1 (bits 39:32)
{0x00, 0x00, 0x60, 0x80, 0x00}, // MAC addr reg 2 (bits 31:24)
{0x00, 0x00, 0x60, 0xA0, 0x00}, // MAC addr reg 3 (bits 23:16)
{0x00, 0x00, 0x60, 0xC0, 0x00}, // MAC addr reg 4 (bits 15:08)
{0x00, 0x00, 0x60, 0xE0, 0x00}  // MAC addr reg 5 (bits 07:00)
};

static const int CHANNEL = 1;

uchar spi_buffer[10];

#define spi_mux_sel	24
#define spi_cs1_n	7

uchar read_MAC_reg(uint reg_addr)
{
   // fill buffer with command & reg addr
   uint i = 0;
   spi_buffer[0] = 0x60; //load read command
   i=1;
   while (i<5) {         //load MAC register addr
     spi_buffer[i]=MAC_reg_addr[reg_addr][i];
     i++;
   }

/*  uncomment this code for diag info
   // write buffer contents to console
   printf("SPI output\n");
   for (i=0; i<5; i++)
     printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
   printf("\n");
*/

   // write buffer to SPI port
   wiringPiSPIDataRW(CHANNEL, spi_buffer, 5); // output 5 bytes

   return spi_buffer[4];
}
   
int main(void)
{
   uint fd;

// Init wiringPi bitwise interface.

  if(wiringPiSetup() == -1){ // initialize wiringPi failed
    printf("setup wiringPi failed !");
    return EXIT_FAILURE;
  }

  uint buffer[6];
  spi_read_reg(0x0302, buffer, 6);
  spi_teardown();

  // write buffer contents to console
  printf("Switch MAC addr = ");
  for (uint index=0; index<6; index++) {
    printf("%02X%c",buffer[index],(index==5)?'\n':':');
  }

  return EXIT_SUCCESS;
}

