#include <assert.h>
#include "freelist.h"
#include "entity.h"

void exd_freelist_init(exd_freelist_t * list, allocator_t * mem)
{
	list->first_free_idx = EXD_MAX_ENTITIES - 1;

	list->free_entity_slots = allocator_malloc(mem, u16, EXD_MAX_ENTITIES);
	for (size_t i = 0; i < EXD_MAX_ENTITIES; ++i)
	{
		//We want our array filled backwards with an INVALID_ENTITY at slot 0 on the free list
		//Example array: [0, 8191, 8190, 8189.....3, 2, 1]
		//We pop elements off the back and then push them back onto the back. We're out of IDs if we encounter ID 0
		list->free_entity_slots[i] = (EXD_MAX_ENTITIES - i) & EXD_MAX_ENTITIES_MASK;
	}
}

u16 exd_freelist_get_free_id(exd_freelist_t * list)
{
	u16 target_id = list->free_entity_slots[list->first_free_idx];
	assert(target_id != 0);
	list->first_free_idx--;

	return target_id;
}

void exd_freelist_release_id(exd_freelist_t * list, u16 id)
{
	assert(list->first_free_idx + 1 < EXD_MAX_ENTITIES && id < EXD_MAX_ENTITIES);

	list->first_free_idx++;
	list->free_entity_slots[list->first_free_idx] = id;
}

