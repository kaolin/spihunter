////////////////////////////////////////////////////////////
//  dpmib.c
//  dump  MIB counter from KSZ9896 switch
//  b.r.koball
//  V1.3
//  5 Nov 18  
//
// compile with: gcc -o rdmib rdmib.c -lwiringPi 
//
// call: rdmib <port number 1-6> <MIB counter number>  
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

#ifdef DEBUG
// write buffer contents to console
	printf("SPI output - read reg\n");
	for (i=0; i<5; i++)
		printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
	printf("\n");
#endif


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

#ifdef DEBUG
// write buffer contents to console
	printf("SPI output - write reg\n");
	for (i=0; i<5; i++)
		printf("spi_buffer[%i]= 0x%02X\n",i,spi_buffer[i]);
	printf("\n");
#endif

// write buffer to SPI port
	wiringPiSPIDataRW(CHANNEL, spi_buffer, 5); // output 5 bytes
// reg value returned in spi_buffer[4]
	return spi_buffer[4];
}

uint read_reg32(uint regadr) { 
	uint i, regval = 0;

	for (i=0; i<3; i++) {
		regval += read_reg(regadr);
		regval <<= 8;
		regadr++;
	}
	regval += read_reg(regadr);

#ifdef DEBUG
	printf("regadr = 0x%04x reg32 value = 0x%08x\n",regadr, regval);
#endif

	return regval;
}

int rd_mib(uint portn, uint cntrn) {
	uint fd, i, adr, regadr, regval, cntrval;

// read port MIB status register to be sure no pending operation

	regadr = 0x0500;			// base addr for Port MIB status reg
	regadr |= ((portn & 0x0F) << 12 );	// put port number in bits 15:12
	while (read_reg32(regadr) & 0x02000000); // check for read operation in progress


// write MIB read enable bit & MIB counter index

	write_reg(regadr, 0x02);		// start MIB read & disable counter flush & reeze
	write_reg(regadr+1, (cntrn & 0xFF));	// MIB counter index
	write_reg(regadr+2, 0x00);
	write_reg(regadr+3, 0x00);

	while (read_reg32(regadr) & 0x02000000); // wait for read operation done

	regadr = 0x0504;			// base addr for Port MIB counter value reg
	regadr |= ((portn & 0x0F) << 12 );	// put port number in bits 15:12
	cntrval = read_reg32(regadr);		// read 32 bit counter value

	printf("port %d MIB counter 0x%02x value = %d\n", portn, cntrn, cntrval);

}

int main(int argc, char *argv[])
{
	uint fd, i, adr, regadr, regval, portn, cntrn, cntrval;
	uchar buf32[4];
	if (argc != 2) {
                fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
                exit(EXIT_FAILURE);
	}

	spiadr = 0;

	sscanf(argv[1], "%d", &portn);

	printf("port addr = %d\n",portn);
	if (portn < 1 || portn >6){
        	fprintf(stderr, "port number must be between 1 and 6\n");
                exit(EXIT_FAILURE);
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

	for(i=0; i<0x20; i++) 
	rd_mib(portn, i);
	
	digitalWrite(spi_mux_sel, LOW);

	return 0;
}

