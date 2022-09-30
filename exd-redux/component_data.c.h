#include "component_data.h"
#include <inttypes.h>
#include <assert.h>

static inline void * exd_component_internal_get_element_location(exd_component_t * comps, exd_entity_t id)
{
	void * base_addr = comps->comps;
	size_t target_index = EXD_ENTITY_ID(id);
	size_t per_element_size = comps->per_element_size;

	uintptr_t location = EXD_CAST(uintptr_t, base_addr);
	uintptr_t amount_to_advance = per_element_size * target_index;

	uintptr_t DEBUG_maximum_safe_offset = per_element_size * EXD_MAX_ENTITIES;
	assert(amount_to_advance < DEBUG_maximum_safe_offset);

	uintptr_t new_location = location + amount_to_advance;

	return EXD_CAST(void *, new_location);
}

void exd_component_init(exd_component_t * comps, size_t per_element_size, allocator_t * mem)
{
	if (per_element_size == 0)
	{
		//This is a tag component only with no storage
		comps->comps = NULL;
	}
	else
	{
		//This is a normal component storage and has elements
		comps->comps = allocator_aligned_malloc(mem, 16, per_element_size, EXD_MAX_ENTITIES);
	}
	exd_entset_init(&comps->in_use_components, mem);
	comps->per_element_size = per_element_size;
}

void exd_component_init_as_tag(exd_component_t * comps, allocator_t * mem)
{
	exd_component_init(comps, 0, mem);
}

void const * exd_component_untyped_get(exd_component_t * comps, exd_entity_t id)
{
	assert(EXD_ENTITY_ID(id) != EXD_INVALID_ENTITY);
	assert(exd_entset_slot_is_set(&comps->in_use_components, id));

	return (void const *)exd_component_internal_get_element_location(comps, id);
}

void * exd_component_untyped_get_mut(exd_component_t * comps, exd_entity_t id)
{
	assert(EXD_ENTITY_ID(id) != EXD_INVALID_ENTITY);
	assert(exd_entset_slot_is_set(&comps->in_use_components, id));

	return exd_component_internal_get_element_location(comps, id);
}

void * exd_component_untyped_set(exd_component_t * comps, exd_entity_t id)
{
	assert(EXD_ENTITY_ID(id) < EXD_MAX_ENTITIES);
	//Note: Setting a component a second time before killing it is probably not intended behavior,
	//so assert that the slot is currently inactive
	assert(exd_entset_slot_is_not_set(&comps->in_use_components, id));

	exd_entset_set_slot(&comps->in_use_components, id);

	return exd_component_internal_get_element_location(comps, id);
}

void exd_component_clear(exd_component_t * comps, exd_entity_t id)
{
	assert(EXD_ENTITY_ID(id) < EXD_MAX_ENTITIES && id != EXD_INVALID_ENTITY);
	//Note: It's ok to clear an entity even if it doesn't currently have anything
	//This probably makes entity deletion easier

	exd_entset_clear_slot(&comps->in_use_components, id);
}

bool exd_component_has(exd_component_t * comps, exd_entity_t id)
{
	assert(EXD_ENTITY_ID(id) < EXD_MAX_ENTITIES && id != EXD_INVALID_ENTITY);

	return exd_entset_slot_is_set(&comps->in_use_components, id);
}
