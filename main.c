#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define CPUID_LEN	16
#define OUTPUT_LEN	6
#define TEST_ARRAY	0xFF

extern uint16_t crc16_ccitt(uint16_t crc_start, unsigned char *buf, int len);
extern unsigned int crc8(unsigned int crc, const unsigned char *vptr, int len);

/* base value
 * 54 4d 53 36
 * 33 30 2e 30
 * 30 00 00 00
 * 00 00 00 00 */
static uint8_t cpuid_base[16] =
	{ 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x30,
	0x30, 0x2e, 0x30, 0x33,
	0x36, 0x53, 0x4d, 0x54};

int main(int argc, char **argv)
{
	uint8_t cpuid[CPUID_LEN];
	uint8_t low[CPUID_LEN/2];
	uint8_t high[CPUID_LEN/2];
	unsigned int temp;
	uint64_t i;
	uint32_t base;
	/*
	char temp_str[OUTPUT_LEN*2];
	uint64_t num_array, start_array;
	*/
	FILE *fp;

	/* init buffer */
	memset(cpuid, 0x0, CPUID_LEN);
	memcpy(cpuid, cpuid_base, 16);

	temp = 0ul;
	base = 0;

	fp = fopen("mac_generate.txt", "w");
	if (fp == NULL) {
		printf("file open error!\n");
		return 0;
	}

	while(1) {

		/* copy 4bytes to cpuid[0] ~ cpuid[3] */
		memcpy(cpuid, (uint8_t *)&base, 4); /* 4 bytes */

		/* rearrange cpuid as 8bytes unit */
		for (i = 0; i < CPUID_LEN/2; i++) {
			low[i] = cpuid[1 + (i << 1)];
			high[i] = cpuid[i << 1];
		}

		/* calculate crc16 using low : 8byte input -> 2byte output */
		temp = crc16_ccitt(0, low, 8);

		/* calculate crc8 using high : 8byte input -> 1byte output */
		temp |= (unsigned int)crc8(temp, high, 8) << 16;

		/* translate to string */
		printf("cpuid 0x");
		for (i = 16;i > 0;i--) {
			printf("%x", cpuid[i - 1]);
			fprintf(fp, "%x", cpuid[i - 1]);
		}
		printf("\n");

		printf("low 0x");
		for (i = 0;i < 8;i++)
			printf("%x", low[i]);
		printf("\n");
		printf("high 0x");
		for (i = 0;i < 8;i++)
			printf("%x", high[i]);
		printf("\n");

		printf("temp 0x%x\n", temp);
		fprintf(fp, ",%6x\n", temp);

		if (base == 0xFFFF)
			break;

		base++;
	}

	/* now compare all of temp array */
/*
	num_array = TEST_ARRAY;
	start_array = 0;

	while(1) {

		if (start_array == num_array)
			break;

		printf("start_array %ld\n", start_array);

		for (i = 1;i < (num_array - start_array);i++) {
			printf("%ld %s %s\n", i, temp_str[start_array], temp_str[start_array+i]);
			if(!strcmp(temp_str[start_array], temp_str[start_array + i])) {
				printf("same pattern!!! ");
				printf("%s / %s\n", temp_str[start_array], temp_str[start_array + i]);
			}

		}
		start_array++;
	}
*/

	fclose(fp);

	return 0;
}

