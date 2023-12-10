#include "utf8.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/*
Our guide table:

    Unicode Range       | UTF-8 Encoding
    --------------------|------------------------------------
    U+0000  -> U+007f   | 0xxxxxxx
    U+0080  -> U+07ff   | 110xxxxx 10xxxxxx
    U+0800  -> U+ffff   | 1110xxxx 10xxxxxx 10xxxxxx
    U+10000 -> U+10fff  | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

Where x is the bits of the unicode character, filled from right to left

    10xxxxxx = 0x80 | (x & 0x3f)
    110xxxxx = 0xc0 | (x & 0x1f)
    1110xxxx = 0xe0 | (x & 0x0f)
    11110xxx = 0xf0 | (x & 0x07)

The replacement character:

    11101111 10111111 10111101
    0xef     0xbf     0xbd

*/

uint8_t utf8_encode_char(unicode utf, uint8_t* out) {
	// 1 byte
	if (utf <= 0x7f) {
		out[0] = (uint8_t) utf;
		return 1;
	}

	// 2 bytes
	if (utf <= 0x7ff) {
		out[0] = (uint8_t) ((utf >> 6) & 0x1f) | 0xc0;
		out[1] = (uint8_t) ( utf       & 0x3f) | 0x80;
		return 2;
	}

	// 3 bytes
	if (utf <= 0xffff) {
		out[0] = (uint8_t) ((utf >> 12) & 0x0f) | 0xe0;
		out[1] = (uint8_t) ((utf >> 6 ) & 0x3f) | 0x80;
		out[2] = (uint8_t) ( utf        & 0x3f) | 0x80;
		return 3;
	}

	// 4 bytes
	if (utf <= 0x10ffff) {
		out[0] = (uint8_t) ((utf >> 18) & 0x07) | 0xf0;
		out[1] = (uint8_t) ((utf >> 12) & 0x3f) | 0x80;
		out[2] = (uint8_t) ((utf >> 6 ) & 0x3f) | 0x80;
		out[3] = (uint8_t) ( utf        & 0x3f) | 0x80;
		return 4;
	}

	// replacement character
	out[0] = 0xef;
	out[1] = 0xbf;
	out[2] = 0xbd;
	return 0;
}

uint8_t utf8_decode_char(uint8_t* utf, unicode* out) {
	// 1 byte
	if ((utf[0] & 0x80) == 0) {
		*out = utf[0];
		return 1;
	}

	// 2 bytes
	if ((utf[0] & 0xe0) == 0xc0) {
		// validate byte sequence
		if ((utf[1] & 0xc0) == 0x80) {

			// decode the character
			*out  = (utf[0] & 0x1f) << 6;
			*out |=  utf[1] & 0x3f;

		} else *out = 0xefbfbd;
		return 2;
	}

	// 3 bytes
	if ((utf[0] & 0xf0) == 0xe0) {
		// validate byte sequence
		if ((utf[1] & 0xc0) == 0x80 &&
			(utf[2] & 0xc0) == 0x80
		) {

			// decode the character
			*out  = (utf[0] & 0x0f) << 12;
			*out |= (utf[1] & 0x3f) << 6;
			*out |=  utf[2] & 0x3f;

		} else *out = 0xefbfbd;
		return 3;
	}

	// 4 bytes
	if ((utf[0] & 0xf8) == 0xf0) {
		// validate byte sequence
		if ((utf[1] & 0xc0) == 0x80 &&
			(utf[2] & 0xc0) == 0x80 &&
			(utf[3] & 0xc0) == 0x80
		) {

			// decode the character
			*out  = (utf[0] & 0x07) << 18;
			*out |= (utf[1] & 0x3f) << 12;
			*out |= (utf[2] & 0x3f) << 6;
			*out |=  utf[3] & 0x3f;

		} else *out = 0xefbfbd;
		return 4;
	}

	// replacement character
	*out = 0xefbfbd;
	return 0;
}

uint8_t* utf8_encode_str(unicode* str, size_t len, size_t* out_size) {
	uint8_t* out = (uint8_t*) malloc(sizeof(uint8_t) * 4 * len);

	// out of memory
	if (!out) return NULL;

	// iterate through characters
	for (size_t i = 0; i < len; i++) {
		// encode the utf-8 character
		uint8_t bytesProduced = utf8_encode_char(str[i], out + *out_size);
		*out_size += (bytesProduced == 0) ? 3 : bytesProduced;
	}

	return out;
}

unicode* utf8_decode_str(uint8_t* raw, size_t len, size_t* out_size) {
	size_t buffer = 2;
	unicode* out = (unicode*) malloc(sizeof(unicode) * buffer);

	// out of memory
	if (!out) return NULL;

	// iterate through the bytes
	for (size_t i = 0; i < len; ) {
		// resize the buffer
		if (buffer <= *out_size) {
			buffer *= 2;
			out = (unicode*) realloc(out, sizeof(uint8_t) * buffer);

			// out of memory
			if (!out) {
				free(out);
				return NULL;
			}

		}

		// decode the utf-8 character
		uint8_t bytesConsumed = utf8_decode_char(raw + i, out + *out_size);
		bytesConsumed = (bytesConsumed == 0) ? 1 : bytesConsumed;

		i += bytesConsumed;
		(*out_size)++;
	}

	return out;
}
