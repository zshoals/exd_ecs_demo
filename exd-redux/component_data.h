#pragma once

#include <stddef.h>
#include "tectonic/ds/allocator.h"
#include "exd-common.h"
#include "entity_list.h"
#include "entity.h"
#include "entset.h"

typedef struct exd_component
{
	void * comps;
	exd_entset_t in_use_components;
	size_t per_element_size;
}
exd_component_t;

void exd_component_init(exd_component_t * comps, size_t per_element_size, allocator_t * mem);
void exd_component_init_as_tag(exd_component_t * comps, allocator_t * mem);

void const * exd_component_untyped_get(exd_component_t * comps, exd_entity_t id);
void * exd_component_untyped_get_mut(exd_component_t * comps, exd_entity_t id);

void * exd_component_untyped_set(exd_component_t * comps, exd_entity_t id);

void exd_component_clear(exd_component_t * comps, exd_entity_t id);

bool exd_component_has(exd_component_t * comps, exd_entity_t id);

#define exd_component_get(COMPS_PTR, TYPE, ENTITY_ID) (TYPE const *)(exd_component_untyped_get(COMPS_PTR, ENTITY_ID))
#define exd_component_get_mut(COMPS_PTR, TYPE, ENTITY_ID) (TYPE *)(exd_component_untyped_get_mut(COMPS_PTR, ENTITY_ID))

#define exd_component_set(COMPS_PTR, TYPE, ENTITY_ID) (TYPE *)(exd_component_untyped_set(COMPS_PTR, ENTITY_ID))
