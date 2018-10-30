////////////////////////////////////////////////////////////
//  wrmac.c
//  wrte MAC address to KSZ9896 switch
//  b.r.koball
//  V1.2
//  10 Oct 18  
//
// compile with: gcc -o wrmac wrmac.c -lwiringPi 
//
////////////////////////////////////////////////////////////

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>

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
uint buffer[100];

#define DIAG

#define spi_mux_sel	24
#define spi_cs1_n	7

uchar read_MAC_reg(uint reg_addr, uint mac_addr_val)
{
   // fill buffer with command & reg addr
   uint i = 0;
   spi_buffer[0] = 0x60; //load read command
   i=1;
   while (i<5) {         //load MAC register addr
     spi_buffer[i]=MAC_reg_addr[reg_addr][i];
     i++;
   }

#ifdef DIAG

   // write buffer contents to console
   printf("SPI output\n");
   for (i=0; i<5; i++)
     printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
   printf("\n");

#endif

   // write buffer to SPI port
   wiringPiSPIDataRW(CHANNEL, spi_buffer, 5); // output 5 bytes

   return spi_buffer[4];
}

uchar write_MAC_reg(uint reg_addr)
{
   // fill buffer with command & reg addr
   uint i = 0;
   spi_buffer[0] = 0x40; //load write command
   i=1;
   while (i<4) {         //load MAC register addr
     spi_buffer[i]=MAC_reg_addr[reg_addr][i];
     i++;
   }
   spi_buffer[4]=buffer[reg_addr];

#ifdef DIAG
   
   // write buffer contents to console
   printf("SPI output\n");
   for (i=0; i<5; i++)
     printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
   printf("\n");

#endif 

   // write buffer to SPI port
   wiringPiSPIDataRW(CHANNEL, spi_buffer, 5); // output 5 bytes

   return spi_buffer[4];
}
   

int main(int argc, char *argv[])
{
  uint fd, i, c;
  uchar index;

  if (argc < 2) {
    printf("missing arguments\n");
    return -1;
  }

  if (argc != 7) {
    printf("incorrect number of digits\n");
    return -1;
  }

  for (i = 1; i < 7; i++)
  {
    sscanf(argv[i], "%x", &buffer[i-1]);
    if (buffer[i-1] > 255)
    {
      printf( "value out of range (must bebetween 0x00 - 0xFF)\n");
      return -1;
    }
  }

  printf("I'm going to write the following MAC address to the switch:\n");

  for (i = 0; i < 5; i++)
  {
    printf("%02X:",buffer[i]);
  }
  printf("%02X\n",buffer[5]);
  
  //printf("OK (y/n)?");
  //c = getc(stdin);

  //if (c != 'y') {
  //  printf("MAC addr write ABORTED\n");
  //  return -1;
  //}

// Init wiringPi bitwise interface.

  if(wiringPiSetup() == -1){ // initialize wiringPi failed
    printf("setup wiringPi failed !");
    return 1;
  }

  pinMode(spi_mux_sel, OUTPUT);

  digitalWrite(spi_mux_sel, HIGH);


// Init wiringPi spi interface.
// CHANNEL = chip select,
// bus speed = 500KHz

  fd = wiringPiSPISetup(CHANNEL, 500000);

// send write command to switch and read MAC reg contents

   printf("writing MAC addr to switch\n");
   for (i=0; i < 6; i++)
     write_MAC_reg(i);


  printf("reading back MAC addr to confirm:\n");

// send read command to switch and read MAC reg contents

   for (i=0; i<6; i++)   
     buffer[i] = read_MAC_reg(i,4);

// write buffer contents to console

  printf("Switch MAC addr = ");
  for (index=0; index<5; index++)
{
    printf("%02X:",buffer[index]);
}
  index++;
  printf("%02X\n",buffer[5]);

  digitalWrite(spi_mux_sel, LOW);
  return 0;
}

