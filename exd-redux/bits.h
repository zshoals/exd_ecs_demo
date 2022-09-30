#pragma once

#include <assert.h>
#include "exd-common.h"

//=================================
//
//      64 bit operations
//
//=================================

static inline u64 exd_bits64_and(u64 a, u64 b) { return a & b; }
static inline u64 exd_bits64_or(u64 a, u64 b) { return a | b; }
static inline u64 exd_bits64_not(u64 a) { return ~a; }
static inline u64 exd_bits64_rotate_left(u64 a, u8 rotL) { return (a << rotL); }
static inline u64 exd_bits64_rotate_right(u64 a, u8 rotR) { return (a >> rotR); }
static inline u64 exd_bits64_mask_from_rotate_left(u8 shift)
{
	// assert(shift != 0);
	return exd_bits64_rotate_left(1, shift) - 1;
}
static inline u64 exd_bits64_extract_lower_bits(u64 a, u8 pivot)
{
	u64 mask = exd_bits64_mask_from_rotate_left(pivot);
	return exd_bits64_and(a, mask);
}
static inline u64 exd_bits64_extract_upper_bits(u64 a, u8 pivot)
{
	return exd_bits64_rotate_right(a, pivot);
}
static inline u64 exd_bits64_set_lower(u64 a, u64 patch_in, u8 pivot)
{
	u64 shifted_value = exd_bits64_rotate_left(a, pivot);
	u64 filled_lower_range = exd_bits64_extract_lower_bits(patch_in, pivot);

	return exd_bits64_or(shifted_value, filled_lower_range);
}
static inline u64 exd_bits64_set_upper(u64 a, u64 patch_in, u8 pivot)
{
	u64 lower_bits = exd_bits64_extract_lower_bits(a, pivot);
	u64 to_upper = exd_bits64_rotate_left(patch_in, pivot);

	return exd_bits64_or(to_upper, lower_bits);
}

static inline u64 exd_bits64_increment_upper(u64 a, u8 pivot)
{
	u64 upper_bits = exd_bits64_extract_upper_bits(a, pivot);
	upper_bits++;
	return exd_bits64_set_upper(a, upper_bits, pivot);
}

//=================================
//
//       32 bit operations
//
//=================================

static inline u32 exd_bits32_and(u32 a, u32 b) { return a & b; }
static inline u32 exd_bits32_or(u32 a, u32 b) { return a | b; }
static inline u32 exd_bits32_not(u32 a) { return ~a; }
static inline u32 exd_bits32_mask_from_rotate_left(u8 shift)
{
	// assert(shift != 0);
	return exd_bits32_rotate_left(1, shift) - 1;
}
static inline u32 exd_bits32_rotate_left(u32 a, u8 rotL) { return (a << rotL); }
static inline u32 exd_bits32_rotate_right(u32 a, u8 rotR) { return (a >> rotR); }
static inline u32 exd_bits32_extract_lower_bits(u32 a, u8 pivot)
{
	u32 mask = exd_bits32_mask_from_rotate_left(pivot);
	return exd_bits32_and(a, mask);
}
static inline u32 exd_bits32_extract_upper_bits(u32 a, u8 pivot)
{
	return exd_bits32_rotate_right(a, pivot);
}
static inline u32 exd_bits32_set_lower(u32 a, u32 patch_in, u8 pivot)
{
	u32 shifted_value = exd_bits32_rotate_left(a, pivot);
	u32 filled_lower_range = exd_bits32_extract_lower_bits(patch_in, pivot);

	return exd_bits32_or(shifted_value, filled_lower_range);
}
static inline u32 exd_bits32_set_upper(u32 a, u32 patch_in, u8 pivot)
{
	u32 lower_bits = exd_bits32_extract_lower_bits(a, pivot);
	u32 to_upper = exd_bits32_rotate_left(patch_in, pivot);

	return exd_bits32_or(to_upper, lower_bits);
}

static inline u32 exd_bits32_increment_upper(u32 a, u8 pivot)
{
	u32 upper_bits = exd_bits32_extract_upper_bits(a, pivot);
	upper_bits++;
	return exd_bits32_set_upper(a, upper_bits, pivot);
}
