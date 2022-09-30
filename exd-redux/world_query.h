#pragma once

#include "exd-config.h"

#include "tectonic/ds/allocator.h"
#include "world.h"
#include "exd-common.h"
#include "iterable_entity.h"

typedef struct exd_query
{
	exd_entset_t matcher;
	exd_world_t * world;
}
exd_query_t;

typedef struct exd_query_optional_group
{
	exd_entset_t matcher;
	exd_entset_t * optionals[EXD_QUERY_MAX_OPTIONAL_COMPONENT_MERGE];
	size_t optionals_count;
	exd_world_t * world;
}
exd_query_optional_group_t;

typedef struct exd_query_iter
{
	exd_query_t * q;
	u32 current_index;
}
exd_query_iter_t;

void exd_query_init_from(exd_query_t * q, exd_world_t * world, allocator_t * mem);
#define exd_query_init_fast(QUERY_PTR, ALLOCATOR_PTR) exd_query_init_from(QUERY_PTR, exd_world_get_global_world_for_shorthand_access(), ALLOCATOR_PTR)
void exd_query_include(exd_query_t * q, size_t component_index);
void exd_query_exclude(exd_query_t * q, size_t component_index);
void exd_query_merge_optional_group(exd_query_t * q, exd_query_optional_group_t * optionals);
exd_query_iter_t exd_query_compile(exd_query_t * q);

void exd_query_optional_group_init(exd_query_optional_group_t * optionals, exd_world_t * world, allocator_t * mem);
#define exd_query_optional_group_init_fast(OPTIONAL_GROUP_PTR, ALLOCATOR_PTR) exd_query_optional_group_init(OPTIONAL_GROUP_PTR, exd_world_get_global_world_for_shorthand_access(), ALLOCATOR_PTR)
void exd_query_optional_group_push(exd_query_optional_group_t * optionals, size_t component_index);
void exd_query_optional_group_compile(exd_query_optional_group_t * optionals);

//===========================================
//VVVVVV Query iteration convenience VVVVVVVV
//===========================================

exd_iterable_entity_t exd_query_iter_next(exd_query_iter_t * it);

#define foreach_entity(ENT_HANDLE, ENTITY_ITER_PTR)\
	for(exd_iterable_entity_t ENT_HANDLE = exd_query_iter_next(ENTITY_ITER_PTR); exd_iterable_entity_to_normal_entity(ENT_HANDLE); ENT_HANDLE = exd_query_iter_next(ENTITY_ITER_PTR))
