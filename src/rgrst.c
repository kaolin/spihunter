////////////////////////////////////////////////////////////
//  rgrst.c
//  assert KSZ9031 rgmii reset
//  b.r.koball
//  V1.0
//  30 Oct 18  
//
// compile with: gcc <pgm_name>.c -lwiringPi 
//
////////////////////////////////////////////////////////////

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

#define spi_mux_sel	24
#define spi_cs1_n	7
#define switch_reset    5
#define rgmii_reset     4

int main(void)
{
   uint fd;
   uchar buffer[100], index;

// Init wiringPi bitwise interface.

  if(wiringPiSetup() == -1){ // initialize wiringPi failed
    printf("setup wiringPi failed !");
    return 1;
  }


  pinMode(rgmii_reset, OUTPUT);
  
  printf("asserting rgmii reset for 10 ms\n");

  delay(100);

  digitalWrite(rgmii_reset, LOW);

  delay(10);

  digitalWrite(rgmii_reset, LOW);
  
  return 0;
}

