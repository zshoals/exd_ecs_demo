#pragma once

#include "tectonic/ds/allocator.h"
#include "exd-common.h"
#include "entity.h"

#define EXD_ENTSET_BITWIDTH_SHIFT 5
#define EXD_ENTSET_BITWIDTH (1 << EXD_ENTSET_BITWIDTH_SHIFT)
#define EXD_ENTSET_ELEMENT_COUNT (EXD_MAX_ENTITIES / EXD_ENTSET_BITWIDTH)

typedef struct exd_entset
{
	u32 * bitset;
}
exd_entset_t;

void exd_entset_init(exd_entset_t * ents, allocator_t * mem);
void exd_entset_copy_data_from(exd_entset_t * destination, exd_entset_t * source);

void exd_entset_set_slot(exd_entset_t * ents, exd_entity_t slot);
void exd_entset_clear_slot(exd_entset_t * ents, exd_entity_t slot);

bool exd_entset_block_is_empty(exd_entset_t * ents, exd_entity_t slot);
bool exd_entset_block_is_full(exd_entset_t * ents, exd_entity_t slot);

void exd_entset_and(exd_entset_t * destination, exd_entset_t * source);
void exd_entset_or(exd_entset_t * destination, exd_entset_t * source);
void exd_entset_not(exd_entset_t * destination, exd_entset_t * source);

bool exd_entset_slot_is_set(exd_entset_t * ents, exd_entity_t slot);
bool exd_entset_slot_is_not_set(exd_entset_t * ents, exd_entity_t slot);
