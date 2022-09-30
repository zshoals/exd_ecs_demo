#pragma once

#include "exd-common.h"

static inline u32 exd_math32_pow2_divide(u32 value, u8 shift) { return value >> shift; }
static inline u32 exd_math32_pow2_modulo(u32 value, u8 shift) { return value & ((1 << shift) - 1); }

static inline u64 exd_math64_pow2_divide(u64 value, u8 shift) { return value >> shift; }
static inline u64 exd_math64_pow2_modulo(u64 value, u8 shift) { return value & ((1 << shift) - 1); }

#define exd_math_pow2_divide(VALUE, SHIFT) ((VALUE) >> (SHIFT))
#define exd_math_pow2_modulo(VALUE, SHIFT) ( (VALUE) & ((1 << (SHIFT)) - 1) )
