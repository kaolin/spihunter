////////////////////////////////////////////////////////////
//  read_mac.c
//  read MAC address of KSZ9896 switch & write it to console
//  b.r.koball
//  V1.2
//  9 Oct 18  
//
// compile with: gcc spi_test.c -lwiringPi 
//
////////////////////////////////////////////////////////////

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

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
   uchar buffer[100], index;

// Init wiringPi bitwise interface.

  if(wiringPiSetup() == -1){ // initialize wiringPi failed
    printf("setup wiringPi failed !");
    return 1;
  }

  pinMode(spi_mux_sel, OUTPUT);
//  pinMode(spi_cs1_n, OUTPUT);

  digitalWrite(spi_mux_sel, HIGH);

/*
while (1) {
  digitalWrite(spi_cs1_n, LOW);
    printf("spi_cs1_n = LOW\n");
  delay(100);
  digitalWrite(spi_cs1_n, HIGH);
    printf("spi_cs1_n = HIGH\n");
  delay(100);
}
*/

// Init wiringPi spi interface.
// CHANNEL = chip select,
// bus speed = 500KHz

  fd = wiringPiSPISetup(CHANNEL, 500000);

// send read command to switch and read MAC reg contents

   buffer[0] = read_MAC_reg(0);
   buffer[1] = read_MAC_reg(1);
   buffer[2] = read_MAC_reg(2);
   buffer[3] = read_MAC_reg(3);
   buffer[4] = read_MAC_reg(4);
   buffer[5] = read_MAC_reg(5);


// write buffer contents to console

  printf("Switch MAC addr = ");
  for (index=0; index<5; index++)
{
    printf("%02X:",buffer[index]);
}
  index++;
  printf("%02X\n",buffer[5]);

  digitalWrite(spi_mux_sel, HIGH)
  return 0;
}

