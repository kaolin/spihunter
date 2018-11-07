////////////////////////////////////////////////////////////
//  rdmac.c
//  read MAC address of KSZ9896 switch & write it to console
//  b.r.koball
//  k.i.fire
//  V2.0
//  2018-11-07
//
// compile with: gcc rdmac.c reg.c -lwiringPi 
//
////////////////////////////////////////////////////////////

#include "reg.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>

int main(void) {
  if(spi_setup() != EXIT_SUCCESS){ // initialize wiringPi failed
    printf("setup wiringPi failed !");
    return EXIT_FAILURE;
  }

  uint buffer[6];
  spi_read_reg(0x0302, buffer, 6);
  spi_teardown();


// write buffer contents to console

  printf("Switch MAC addr = ");
  uint index;
  for (index=0; index<5; index++)
{
    printf("%02X:",buffer[index]);
}
  index++;
  printf("%02X\n",buffer[5]);

  digitalWrite(spi_mux_sel, HIGH);
  return 0;
}

