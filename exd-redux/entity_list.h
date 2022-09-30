#pragma once

#include "tectonic/ds/allocator.h"
#include "exd-common.h"
#include "entity.h"
#include "entset.h"
#include "freelist.h"

typedef struct exd_entity_list
{
	exd_entity_t * entity_references;
	exd_freelist_t available_entities;
	exd_entset_t entity_states;
}
exd_entity_list_t;

void exd_entity_list_init(exd_entity_list_t * manifest, allocator_t * mem);
exd_entity_t exd_entity_list_get_new_entity(exd_entity_list_t * manifest);
void exd_entity_list_free_entity(exd_entity_list_t * manifest, exd_entity_t id);
void exd_entity_list_copy_to_entset(exd_entity_list_t * manifest, exd_entset_t * destination);
bool exd_entity_list_is_entity_valid(exd_entity_list_t * manifest, exd_entity_t id);
exd_entity_t exd_entity_list_resolve_index(exd_entity_list_t * manifest, u16 idx);
