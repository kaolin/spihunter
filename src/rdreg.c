////////////////////////////////////////////////////////////
//  rdreg.c
//  read Management Interface register contents from KSZ9896 switch
//  b.r.koball
//  k.i.fire
//  V2.0
//  2018-11-07
//
// compile with: gcc -o rdreg rdreg.c reg.c -lwiringPi
//
// call: rdreg <reg addr as 4-digit hex> <N of regs to read>
////////////////////////////////////////////////////////////

#include "reg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	uint fd, i, regval, spiadr, spicnt;
	uint fd, i, adr, regval, buf[100], spiadr, spicnt;

	if (argc == 2) {
		sscanf(argv[1], "%x", &spiadr);
		spicnt = 1;
	} else if (argc != 3) {
                fprintf(stderr, "Usage: %s <reg starting addr> <reg count>\n", argv[0]);
                return EXIT_FAILURE;
	} else {
		sscanf(argv[1], "%x", &spiadr);
		sscanf(argv[2], "%d", &spicnt);
	}

	printf("reg addr = %x; reg cnt = %x\n", spiadr, spicnt);

	if (spicnt == 0) {
		fprintf(stderr, "reg count must be non-zero\n");
		return EXIT_FAILURE;
	}

	if (spi_setup() != EXIT_SUCCESS) {
		fprintf(stderr, "unable to setup spi\n");
		return EXIT_FAILURE;
	}

	// read spicnt bytes
	uint buf[spicnt];
	spi_read_reg(spiadr, buf, spicnt);
	for(i = 0; i<spicnt; i++) {
		printf("reg 0x%02x = 0x%02x  0b", spiadr+i, buf[i]);
		printf("%s%s\n", bit_rep[buf[i] >> 4], bit_rep[buf[i] & 0x0F]);
	}	

	spi_teardown();
	return EXIT_SUCCESS;
}
