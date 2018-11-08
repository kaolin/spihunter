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
  for (uint index=0; index<6; index++) {
    printf("%02X%c",buffer[index],(index==5)?'\n':':');
  }

  return EXIT_SUCCESS;
}

