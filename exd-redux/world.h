#pragma once

#include "exd-config.h"

#include "entity_list.h"
#include "component_data.h"
#include "tectonic/ds/allocator.h"
#include "entity.h"
#include "iterable_entity.h"

typedef struct exd_world
{
	exd_entity_list_t entities;
	exd_component_t component_arrays[EXD_MAX_COMPONENTS];
	size_t current_max_components;
	allocator_t * mem;
}
exd_world_t;

typedef enum
{
	EXD_GET_COMP_RESULT_SUCCESS,
	EXD_GET_COMP_RESULT_INVALID_ENTITY,
	EXD_GET_COMP_RESULT_MISSING_COMPONENT
}
exd_world_comp_result_state_e;

typedef struct exd_world_comp_get_result
{
	exd_world_comp_result_state_e result;
	void const * component;
}
exd_world_comp_get_result_t;

typedef struct exd_world_comp_get_mut_result
{
	exd_world_comp_result_state_e result;
	void * component;
}
exd_world_comp_get_mut_result_t;

void exd_world_init(exd_world_t * world, allocator_t * mem);

void exd_world_component_create_sized_component_storage(exd_world_t * world, size_t per_element_size, size_t component_idx);
#define exd_world_component_create_component_storage(WORLD, TYPE, COMPONENT_IDX) exd_world_component_create_sized_component_storage(WORLD, sizeof(TYPE), COMPONENT_IDX)
void exd_world_component_create_tag_storage(exd_world_t * world, size_t component_idx);

bool exd_world_entity_has(exd_world_t * world, size_t component_idx, exd_entity_t ent);
bool exd_world_entity_is_valid(exd_world_t * world, exd_entity_t ent);

exd_entity_t exd_world_entity_new(exd_world_t * world);
void exd_world_entity_kill(exd_world_t * world, exd_entity_t ent);

exd_world_comp_get_result_t exd_world_component_get(exd_world_t * world, size_t component_idx, exd_entity_t ent);
exd_world_comp_get_mut_result_t exd_world_component_get_mut(exd_world_t * world, size_t component_idx, exd_entity_t ent);

//Note: The unsafe component accessors skip the generation check
//These are safe to use as long as they're used by entities resolved by a world query
void const * exd_world_component_get_unsafe(exd_world_t * world, size_t component_idx, exd_iterable_entity_t ent);
void * exd_world_component_get_mut_unsafe(exd_world_t * world, size_t component_idx, exd_iterable_entity_t ent);

void * exd_world_component_set(exd_world_t * world, size_t component_idx, exd_entity_t ent);


//============================================
//Shorthand access functions to save on typing
//============================================

void exd_world_set_global_world_for_shorthand_access(exd_world_t * world);
exd_world_t * exd_world_get_global_world_for_shorthand_access();

#define exd_ent_new() exd_world_entity_new(exd_world_get_global_world_for_shorthand_access())
#define exd_ent_kill(ENTITY) exd_world_entity_kill(exd_world_get_global_world_for_shorthand_access(), ENTITY)
#define exd_ent_iter_to_norm(ITERABLE_ENTITY) exd_iterable_entity_to_normal_entity(ITERABLE_ENTITY)

#define exd_ent_has(COMPONENT_IDX, ENTITY) exd_world_entity_has(exd_world_get_global_world_for_shorthand_access(), COMPONENT_IDX, ENTITY)
#define exd_ent_valid(ENTITY) exd_world_entity_is_valid(exd_world_get_global_world_for_shorthand_access(), ENTITY)

#define exd_comp_get(COMPONENT_IDX, ITERABLE_ENTITY) exd_world_component_get_unsafe(exd_world_get_global_world_for_shorthand_access(), COMPONENT_IDX, ITERABLE_ENTITY)
#define exd_comp_get_mut(COMPONENT_IDX, ITERABLE_ENTITY) exd_world_component_get_mut_unsafe(exd_world_get_global_world_for_shorthand_access(), COMPONENT_IDX, ITERABLE_ENTITY)

#define exd_comp_get_checked(COMPONENT_IDX, ENTITY) exd_world_component_get(exd_world_get_global_world_for_shorthand_access(), COMPONENT_IDX, ENTITY)
#define exd_comp_get_mut_checked(COMPONENT_IDX, ENTITY) exd_world_component_get_mut(exd_world_get_global_world_for_shorthand_access(), COMPONENT_IDX, ENTITY)

#define exd_comp_set(COMPONENT_IDX, ENTITY) exd_world_component_set(exd_world_get_global_world_for_shorthand_access(), COMPONENT_IDX, ENTITY)

//Ideas for additional functions: Report max entities in use, ents free, total component usage, etc.
