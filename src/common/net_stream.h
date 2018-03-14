#ifndef _NET_STREAM_HEADER_H_
#define _NET_STREAM_HEADER_H_
#include <stdint.h>

typedef unsigned char Byte;
typedef int8_t Char;
typedef uint8_t UChar;
typedef int16_t Short;
typedef uint16_t UShort;
typedef int32_t Long;
typedef uint32_t ULong;
typedef int64_t LongLong;
typedef uint64_t ULongLong;
typedef float  Float;
typedef double Double;

uint16_t swap_2(uint16_t v);

uint32_t swap_4(uint32_t v);

uint64_t swap_8(uint64_t v);

bool is_little_endian();

#endif
