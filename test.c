#include "utf8.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

int main(void) {
	printf("===== test1: utf8_encode_char()\n");
	printf("input:    U+24B8\n");
	printf("expected: E2 92 B8\n");
	printf("result:   ");
	uint8_t test1[4];
	size_t test1_out_size = utf8_encode_char(0x24B8, test1);
	for (size_t i = 0; i < test1_out_size; i++) printf("%02X ", test1[i]);
	printf("\n\n");

	printf("===== test2: utf8_decode_char()\n");
	printf("input:    F0 9F 8D A9\n");
	printf("expected: U+1F369\n");
	printf("result:   ");
	unicode test2;
	uint8_t test2_input[4] = { 0xF0, 0x9F, 0x8D, 0xA9 };
	utf8_decode_char(test2_input, &test2);
	printf("U+%04X", test2);
	printf("\n\n");

	printf("===== test3: utf8_encode_str()\n");
	printf("input:    U+24B8 U+1F369 U+1F680 U+1F44B\n");
	printf("expected: E2 92 B8 F0 9F 8D A9 F0 9F 9A 80 F0 9F 91 8B\n");
	printf("result:   ");
	unicode test3_input[4] = { 0x24B8, 0x1F369, 0x1F680, 0x1F44B };
	size_t test3_out_size = 0;
	uint8_t* test3 = utf8_encode_str(test3_input, 4, &test3_out_size);
	for (size_t i = 0; i < test3_out_size; i++) printf("%02X ", test3[i]);
	free(test3);
	printf("\n\n");

	printf("===== test4: utf8_decode_str()\n");
	printf("input:    41 C2 A2 E2 82 AC F0 9F 99 82\n");
	printf("expected: U+0041 U+00A2 U+20AC U+1F642\n");
	printf("result:   ");
	uint8_t test4_input[] = {
		0x41, 0xC2, 0xA2, 0xE2, 0x82, 0xAC, 0xF0, 0x9F, 0x99, 0x82
	};
	size_t test4_out_size = 0;
	unicode* test4 = utf8_decode_str(test4_input, 10, &test4_out_size);
	for (size_t i = 0; i < test4_out_size; i++) printf("U+%04X ", test4[i]);
	free(test4);
	printf("\n");

	return 0;
}
