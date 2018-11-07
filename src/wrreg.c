////////////////////////////////////////////////////////////
//  wrreg.c
//  write Management Interface register contents to KSZ9896 switch
//  b.r.koball
//  k.i.fire
//  V2.0
//  2018-11-01
//
// compile with: gcc -o wrreg wrreg.c reg.c -lwiringPi 
//
// call: wrreg <reg addr as 4-digit hex> <data to write>+
////////////////////////////////////////////////////////////

#include "reg.h"
#include <stdio.h>
#include <string.h>

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
	if(spi_setup() != EXIT_FAILURE){ // initialize wiringPi failed
		fprintf(stderr, "setup wiringPi failed !");
		exit(EXIT_FAILURE);
	}

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

	spi_teardown();
	return EXIT_SUCCESS;
}
