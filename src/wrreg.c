////////////////////////////////////////////////////////////
//  wreg.c
//  write Management Interface register contents to KSZ9896 switch
//  b.r.koball
//  V1.2
//  20 Oct 18  
//
// compile with: gcc -o wreg wreg.c -lwiringPi 
//
// call: wreg <reg addr as 4-digit hex> <data to write> 
////////////////////////////////////////////////////////////

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uchar unsigned char
#define uint unsigned int

uint spiadr, spicnt;
uchar spi_buffer[5];

#define spi_mux_sel	24
#define spi_cs1_n	7

static const int CHANNEL = 1;

const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

uchar read_reg(uint reg_adr)
{
	uint i;
// assemble byte-wise spi serial command in spi_buffer[] array
	spi_buffer[0] = 0x60; // read command
	reg_adr <<= 5;		// align spi address for spi command format
	spi_buffer[3] = (unsigned char) (reg_adr & 0x000000E0);
	reg_adr >>= 8;
	spi_buffer[2] = (unsigned char) (reg_adr & 0x000000FF);
	reg_adr >>= 8;
	spi_buffer[1] = (unsigned char) (reg_adr & 0x000000FF);
	spi_buffer[4] = 0;

/*
// write buffer contents to console
	printf("SPI output\n");
	for (i=0; i<5; i++)
		printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
	printf("\n");
*/

// write buffer to SPI port
	wiringPiSPIDataRW(CHANNEL, spi_buffer, 5); // output 5 bytes
// reg value returned in spi_buffer[4]
	return spi_buffer[4];
}

uchar write_reg(uint reg_adr, uint reg_val)
{
	uint i;
// assemble byte-wise spi serial command in spi_buffer[] array
	spi_buffer[0] = 0x40; // write command
	reg_adr <<= 5;		// align spi address for spi command format
	spi_buffer[3] = (unsigned char) (reg_adr & 0x000000E0);
	reg_adr >>= 8;
	spi_buffer[2] = (unsigned char) (reg_adr & 0x000000FF);
	reg_adr >>= 8;
	spi_buffer[1] = (unsigned char) (reg_adr & 0x000000FF);
	spi_buffer[4] = reg_val;

/*
// write buffer contents to console
	printf("SPI output\n");
	for (i=0; i<5; i++)
		printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
	printf("\n");
*/

// write buffer to SPI port
	wiringPiSPIDataRW(CHANNEL, spi_buffer, 5); // output 5 bytes
// reg value returned in spi_buffer[4]
	return spi_buffer[4];
}

int main(int argc, char *argv[])
{
	uint fd, i, adr, regadr, regval, newregval, buf[100];
	uchar c;
	
	if (argc != 3) {
                fprintf(stderr, "Usage: %s <hex reg addr> <hex reg value>\n", argv[0]);
                exit(EXIT_FAILURE);
	}

	sscanf(argv[1], "%x", &regadr);
	sscanf(argv[2], "%x", &newregval);

// Init wiringPi bitwise interface.
	if(wiringPiSetup() == -1){ // initialize wiringPi failed
	fprintf(stderr, "setup wiringPi failed !");
	exit(EXIT_FAILURE);
	}
	pinMode(spi_mux_sel, OUTPUT);
	digitalWrite(spi_mux_sel, HIGH);

// Init wiringPi spi interface.
// CHANNEL = chip select,
// bus speed = 500KHz

	fd = wiringPiSPISetup(CHANNEL, 500000);


	regval = read_reg(regadr);
	printf("Reg 0x%04X value is now:  0x%02X   ", regadr, regval);
	printf("0b%s%s\n", bit_rep[regval >> 4], bit_rep[regval & 0x0F]);
	printf("Reg 0x%04X value will be: 0x%02X   ", regadr, newregval);
	printf("0b%s%s\n", bit_rep[newregval >> 4], bit_rep[newregval & 0x0F]);

 	printf("OK (y/n)?");
	c = getc(stdin);
	if (c != 'y') {
		printf("reg write ABORTED\n");
		exit(EXIT_FAILURE);
	}
	write_reg(regadr, newregval);
	
	digitalWrite(spi_mux_sel, LOW);
	return 0;
}

