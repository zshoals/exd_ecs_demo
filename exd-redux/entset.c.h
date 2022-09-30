#include "entset.h"

#include <string.h>
#include "tectonic/ds/allocator.h"
#include "exd-common.h"
#include "entity.h"
#include "exd-math.h"
#include "bits.h"

static inline u64 exd_entset_internal_compute_array_idx(exd_entity_t id)
{
	return exd_math_pow2_divide(EXD_ENTITY_ID(id), EXD_ENTSET_BITWIDTH_SHIFT);
}

static inline u32 exd_entset_internal_compute_entity_slot_mask(exd_entity_t id)
{
	u8 offset_in_block = exd_math_pow2_modulo(EXD_ENTITY_ID(id), EXD_ENTSET_BITWIDTH_SHIFT);
	u32 slot_mask = exd_bits32_rotate_left(1, offset_in_block);
	return slot_mask;
}

void exd_entset_init(exd_entset_t * ents, allocator_t * mem)
{
	ents->bitset = allocator_malloc(mem, u32, EXD_ENTSET_ELEMENT_COUNT);
	memset( ents->bitset, 0, (EXD_ENTSET_ELEMENT_COUNT * sizeof(u32)) );
}

void exd_entset_copy_data_from(exd_entset_t * destination, exd_entset_t * source)
{
	assert(destination->bitset != NULL && source->bitset != NULL);
	exd_for_range_var(i, EXD_ENTSET_ELEMENT_COUNT)
	{
		destination->bitset[i] = source->bitset[i];
	}
}

void exd_entset_set_slot(exd_entset_t * ents, exd_entity_t slot)
{
	ents->bitset[exd_entset_internal_compute_array_idx(slot)] |= exd_entset_internal_compute_entity_slot_mask(slot);
}

void exd_entset_clear_slot(exd_entset_t * ents, exd_entity_t slot)
{
	ents->bitset[exd_entset_internal_compute_array_idx(slot)] &= ~(exd_entset_internal_compute_entity_slot_mask(slot));
}

bool exd_entset_entity_block_is_empty(exd_entset_t * ents, exd_entity_t slot)
{
	return ents->bitset[exd_entset_internal_compute_array_idx(slot)] == 0;
}

bool exd_entset_block_is_full(exd_entset_t * ents, exd_entity_t slot)
{
	return ents->bitset[exd_entset_internal_compute_array_idx(slot)] == 0xffffffff;
}

void exd_entset_and(exd_entset_t * destination, exd_entset_t * source)
{
	exd_for_range_var(i, EXD_ENTSET_ELEMENT_COUNT)
	{
		destination->bitset[i] &= source->bitset[i];
	}
}

void exd_entset_or(exd_entset_t * destination, exd_entset_t * source)
{
	exd_for_range_var(i, EXD_ENTSET_ELEMENT_COUNT)
	{
		destination->bitset[i] |= source->bitset[i];
	}
}

void exd_entset_not(exd_entset_t * destination, exd_entset_t * source)
{
	exd_for_range_var(i, EXD_ENTSET_ELEMENT_COUNT)
	{
		destination->bitset[i] &= ~(source->bitset[i]);
	}
}

bool exd_entset_slot_is_set(exd_entset_t * ents, exd_entity_t slot)
{
	u32 slot_mask = exd_entset_internal_compute_entity_slot_mask(slot);
	u64 index = exd_entset_internal_compute_array_idx(slot);
	return ents->bitset[index] & slot_mask;
}

bool exd_entset_slot_is_not_set(exd_entset_t * ents, exd_entity_t slot)
{
	return !(exd_entset_slot_is_set(ents, slot));
}

