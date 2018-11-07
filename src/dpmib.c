////////////////////////////////////////////////////////////
//  dpmib.c
//  dump  MIB counter from KSZ9896 switch
//  b.r.koball
//  k.i.fire
//  V3.0
//  8 Nov 18  
//
// compile with: gcc -o dpmib dpmib.c reg.c -lwiringPi 
//
// call: rdmib <port number 1-6> <MIB counter number>  
////////////////////////////////////////////////////////////

#include "reg.h"
#include <stdio.h>

int rd_mib(uint portn, uint cntrn) {
	uint fd, i, adr, regadr, regval;
	bool overflow;

	// read port MIB status register to be sure no pending operation

	regadr = 0x0500;			// base addr for Port MIB status reg
	regadr |= ((portn & 0x0F) << 12 );	// put port number in bits 15:12
	while (spi_read_reg(regadr) & 0x02);    // check for read operation in progress


	// write MIB read enable bit & MIB counter index
	uint reg_values[] = {0x02, cntrn & 0xff, 0x00, 0x00};
	spi_write_reg(regadr, reg_values, 4);

	while ((regval = spi_read_reg(regadr)) & 0x02);    // wait for read operation done

	regadr = 0x0503;			// base addr for Port MIB counter value reg
	regadr |= ((portn & 0x0F) << 12 );	// put port number in bits 15:12

	uint retbuf[] = {0,0,0,0,0};
	overflow = (regval & 0x80);

	spi_read_reg(regadr, retbuf, 5);
	unsigned long cntrval = retbuf[0] & 0x0f;
	cntrval = (cntrval << 8) | retbuf[1];
	cntrval = (cntrval << 8) | retbuf[2];
	cntrval = (cntrval << 8) | retbuf[3];
	cntrval = (cntrval << 8) | retbuf[4];

	printf("port %d  MIB cntr 0x%02x  ovfl = %c  value = %ld\n", portn, cntrn, overflow?'y':'n', cntrval);
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

	if(spi_setup() != EXIT_SUCCESS) {
		fprintf(stderr, "setup wiringPi failed !");
		return EXIT_FAILURE;
	}

	for(i=0; i<0x20; i++) 
		rd_mib(portn, i);

	for(i=0x80; i<0x84; i++) 
		rd_mib(portn, i);
	
	spi_teardown();

	return EXIT_SUCCESS;
}
