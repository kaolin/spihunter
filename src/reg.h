#ifndef reg_h
#define reg_h
////////////////////////////////////////////////////////////
//  reg.h
//  Library Management Interface register contents to KSZ9896 switch
//  b.r.koball
//  k.i.fire
//  V1.0
//  2018-11-07
//
////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define uchar unsigned char
#define uint unsigned int

#define spi_mux_sel	24
#define spi_cs1_n	7

static const int CHANNEL=1;

extern const char *bit_rep[16];

void spi_execute(uchar command, uint reg_adr, uint* reg_val, uint reg_valN);

// various setup for spi bus and our mux selection - returns EXIT_FAILURE OR EXIT_SUCCESS
int spi_setup();

// various teardown for spi bus and our mux selection
void spi_teardown();

// write reg_valN values from reg_val to reg_adr through reg_adr + reg_valN
void spi_write_reg(uint reg_adr, uint* reg_val, uint reg_valN);

// write reg_val to reg_adr
void spi_write_reg(uint reg_adr, uint reg_val);

// read N vals from reg_adr through reg_valN into reg_val
void spi_read_reg(uint reg_adr, uint* reg_val, uint reg_valN);

// read reg_addr
uint spi_read_reg(uint reg_adr);
#endif
