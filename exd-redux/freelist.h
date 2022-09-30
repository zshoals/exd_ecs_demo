
#pragma once

#include "tectonic/ds/allocator.h"
#include "exd-common.h"

typedef struct exd_freelist
{
	u16 first_free_idx;
	u16 * free_entity_slots;
}
exd_freelist_t;

void exd_freelist_init(exd_freelist_t * list, allocator_t * mem);
u16 exd_freelist_get_free_id(exd_freelist_t * list);
void exd_freelist_release_id(exd_freelist_t * list, u16 id);
