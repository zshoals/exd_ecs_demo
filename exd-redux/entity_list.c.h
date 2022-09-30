#include "entity_list.h"
#include "exd-common.h"

exd_entity_t exd_entity_list_resolve_index(exd_entity_list_t * manifest, u16 idx)
{
	//NOTE: An index equivalent to EXD_MAX_ENTITIES is permitted but resolves to an INVALID_ENTITY
	assert(idx <= EXD_MAX_ENTITIES);
	return manifest->entity_references[idx & EXD_MAX_ENTITIES_MASK];
}

void exd_entity_list_init(exd_entity_list_t * manifest, allocator_t * mem)
{
	manifest->entity_references = allocator_malloc(mem, exd_entity_t, EXD_MAX_ENTITIES);
	exd_freelist_init(&manifest->available_entities, mem);
	exd_entset_init(&manifest->entity_states, mem);

	exd_for_range_var(i, EXD_MAX_ENTITIES)
	{
		manifest->entity_references[i] = i;
	}
}

exd_entity_t exd_entity_list_get_new_entity(exd_entity_list_t * manifest)
{
	u16 freeslot = exd_freelist_get_free_id(&manifest->available_entities);
	exd_entity_t id = exd_entity_list_resolve_index(manifest, freeslot);
	exd_entset_set_slot(&manifest->entity_states, id);

	return id;
}

void exd_entity_list_free_entity(exd_entity_list_t * manifest, exd_entity_t id)
{
	u16 slot = EXD_ENTITY_ID(id);
	exd_freelist_release_id(&manifest->available_entities, slot);
	exd_entset_clear_slot(&manifest->entity_states, id);

	exd_entity_t current_reference_value = manifest->entity_references[slot];
	manifest->entity_references[slot] = exd_entity_increment_generation(current_reference_value);
}

//Copy the entity entset, usually used for querying purposes
void exd_entity_list_copy_to_entset(exd_entity_list_t * manifest, exd_entset_t * destination)
{
	exd_for_range_var(i, EXD_ENTSET_ELEMENT_COUNT)
	{
		destination->bitset[i] = manifest->entity_states.bitset[i];
	}
}

bool exd_entity_list_is_entity_valid(exd_entity_list_t * manifest, exd_entity_t id)
{
	bool is_alive = exd_entset_slot_is_set(&manifest->entity_states, id); 
	bool generations_match = manifest->entity_references[EXD_ENTITY_ID(id)] == id;

	return is_alive && generations_match;
}
