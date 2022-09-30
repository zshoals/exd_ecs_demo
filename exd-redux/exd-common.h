#pragma once

#include "exd-config.h"

#define EXD_MAX_ENTITIES_MASK (EXD_MAX_ENTITIES - 1)

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef unsigned char uchar;
typedef unsigned int uint;

#define EXD_CAST(TYPE, VALUE) ((TYPE)(VALUE))

#define exd_for_range(STOP) for (size_t _i = 0; _i < (STOP); ++_i)
#define exd_for_range_var(CAPTURE, STOP) for (size_t (CAPTURE) = 0; (CAPTURE) < (STOP); ++(CAPTURE))
