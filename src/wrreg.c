////////////////////////////////////////////////////////////
//  wrreg.c
//  write Management Interface register contents to KSZ9896 switch
//  b.r.koball
//  k.i.fire
//  V2.0
//  2018-11-01
//
// compile with: gcc -o wrreg wrreg.c -lwiringPi 
//
// call: wrreg <reg addr as 4-digit hex> <data to write>+
////////////////////////////////////////////////////////////

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define HELP

#define uchar unsigned char
#define uint unsigned int

uint spiadr, spicnt;

uchar retval[2];

#define spi_mux_sel	24
#define spi_cs1_n	7

static const int CHANNEL = 1;

const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

void spi_execute(uchar command, uint reg_adr, uint* reg_val, uint reg_valN)
{
	uint spi_bufferN = 4 + (reg_valN ? reg_valN : 1);
	uchar spi_buffer[spi_bufferN];
	// assemble byte-wise spi serial command in spi_buffer[] array
	spi_buffer[0] = command; 
	// align spi address for spi command format
	reg_adr <<= 5;		
	spi_buffer[3] = (unsigned char) (reg_adr & 0x000000E0);
	reg_adr >>= 8;
	spi_buffer[2] = (unsigned char) (reg_adr & 0x000000FF);
	reg_adr >>= 8;
	spi_buffer[1] = (unsigned char) (reg_adr & 0x000000FF);
	if (reg_valN == 0) {
		spi_buffer[4] = 0;
	}
	for (uint i=0; i<reg_valN; i++) {
		spi_buffer[4+i] = reg_val[i];
	}

#ifdef HELP
	// write buffer contents to console
	printf("SPI command\n");
	for (uint i=0; i<spi_bufferN; i++) {
		printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
	}
	printf("\n");
#endif

	wiringPiSPIDataRW(CHANNEL, spi_buffer, spi_bufferN);

#ifdef HELP
	printf("returned\n");
	for (int i=0; i< spi_bufferN;i++) {
		printf("0x%02X\n",spi_buffer[i]);
	}
#endif

}

void spi_write_reg(uint reg_adr, uint* reg_val, uint reg_valN) {
	spi_execute(0x40, reg_adr, reg_val, reg_valN);
}

void spi_write_reg(uint reg_adr, uint reg_val) {
	spi_write_reg(reg_adr, &reg_val, 1);
}

void spi_read_reg(uint reg_adr, uint reg_valN) {
	uint reg_val[reg_valN];
	for (uint i=0; i < reg_valN; i++) {
		reg_val[i] = 0;
	}
	spi_execute(0x60, reg_adr, reg_val, reg_valN);
}

void spi_read_reg(uint reg_adr) {
	spi_execute(0x60, reg_adr, NULL, 0);
}

int main(int argc, char *argv[]) {
	uint fd, i, adr, regadr, regval, newregval1, newregval2, buf[100];
	uchar c;
	
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <hex reg addr> <hex reg value>+\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	uint reg_valN = argc - 2;
	uint reg_val[reg_valN];
	
	sscanf(argv[1], "%x", &regadr);
	for (uint i=0; i < reg_valN; i++) {
		sscanf(argv[i+2], "%x", (reg_val+i));
	}

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

	/*
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
	*/

	printf("%d registers\n",reg_valN);
	spi_write_reg(regadr, reg_val, reg_valN);

	// the below works with chars+4 being the values to read .... but does not
	// store them anywhere, only visible with #define HELP (TODO)
	spi_read_reg(regadr, reg_valN);

	//printf("Reg 0x%04X value is now: 0x%02X   ", regadr, retval[0]);
	//printf("Reg 0x%04X value is now: 0x%02X   ", regadr + 1, retval[1]);
	
	digitalWrite(spi_mux_sel, LOW);
	return EXIT_SUCCESS;
}
