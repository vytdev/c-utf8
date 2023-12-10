/*==============================================================================
 * utf8.h — Custom implementation of UTF-8 encoder and decoder in C
 * Copyright (c) 2023 VYT <https://vytdev.github.io>
 * See: https://github.com/vytdev/c-utf8
 *==============================================================================
 */

#ifndef __VYT_UTF8_H
#define __VYT_UTF8_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t unicode;

/**
 * encode a unicode character into bytes
 * returns the number of bytes produced
 */
uint8_t utf8_encode_char(unicode utf, uint8_t* out);

/**
 * decode UTF-8 bytes into unicode
 * returns the number of bytes consumed
 */
uint8_t utf8_decode_char(uint8_t* utf, unicode* out);

/**
 * encode a unicode string into raw UTF-8 bytes
 * returns the heap byte array
 */
uint8_t* utf8_encode_str(unicode* str, size_t len, size_t* out_size);

/**
 * decode UTF-8 byte array into unicode string
 * returns a heap array of unicode characters.
 */
unicode* utf8_decode_str(uint8_t* raw, size_t len, size_t* out_size);

#endif // __VYT_UTF8_H
