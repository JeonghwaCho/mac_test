#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define CPUID_LEN	16
#define OUTPUT_LEN	6
#define TEST_ARRAY	0xFF

extern uint16_t crc16_ccitt(uint16_t crc_start, unsigned char *buf, int len);
extern unsigned int crc8(unsigned int crc, const unsigned char *vptr, int len);

int main(int argc, char **argv)
{
	uint8_t cpuid[CPUID_LEN];
	uint8_t low[CPUID_LEN/2];
	uint8_t high[CPUID_LEN/2];
	unsigned int temp;
	uint64_t i, loop;
	char temp_str[TEST_ARRAY][OUTPUT_LEN*2];
	uint64_t num_array, start_array;

	/* init buffer */
	memset(cpuid, 0x0, CPUID_LEN);

	/* final value : temp */
	temp = 0ul;
	loop = 0;

	while(1) {

		if (cpuid[0] == 0xff)
			break;

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
		sprintf(temp_str[loop],"%x", temp);

#if 0
		printf("cpuid 0x");
		for (i = 0;i < 16;i++)
			printf("%x", cpuid[i]);
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
		printf("temp (str) %s\n", temp_str[loop]);
#endif

		if (cpuid[0] == 0xff)
			cpuid[1]++;
		else
			cpuid[0]++;

		loop++;
	}

	printf("loop %ld\n", loop);

	/* now compare all of temp array */
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

	return 0;
}

