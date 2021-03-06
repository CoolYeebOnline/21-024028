#pragma once

#include <stdint.h>

//Define our native intergral types to some identifiers that signify their bit size.
//Make sure that our integers are 32 bit, signed unsigned and not system dependent.
typedef uint8_t		u8;		typedef int8_t		s8; //Char
typedef uint16_t	u16;	typedef int16_t		s16;//short
typedef uint32_t	u32;	typedef int32_t		s32;//integer
typedef uint64_t	u64;	typedef int64_t		s64;//long