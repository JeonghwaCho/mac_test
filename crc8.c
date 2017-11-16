/*
 * from u-boot v2017.07 lib/crc.8
 */
/*
 * crc8.c
 * 
 * Computes a 8-bit CRC 
 * 
 */

#include <stdio.h>

#define POLY	(0x1070U << 3)

static unsigned char _crc8(unsigned short data)
{
	int i;

	for (i = 0; i < 8; i++) {
		if (data & 0x8000)
			data = data ^ POLY;
		data = data << 1;
	}

	return (unsigned char)(data >> 8);
}

unsigned int crc8(unsigned int crc, const unsigned char *vptr, int len)
{
	int i;

	for (i = 0; i < len; i++)
		crc = _crc8((crc ^ vptr[i]) << 8);

	return crc;
}
