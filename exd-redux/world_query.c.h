#include "world_query.h"

#include "tectonic/ds/allocator.h"

void exd_query_init_from(exd_query_t * q, exd_world_t * world, allocator_t * mem)
{
	exd_entset_init(&q->matcher, mem);
	exd_entset_copy_data_from(&q->matcher, &world->entities.entity_states);
	q->world = world;
}

void exd_query_include(exd_query_t * q, size_t component_index)
{
	assert(component_index < q->world->current_max_components);
	exd_entset_and(&q->matcher, &q->world->component_arrays[component_index].in_use_components);
}

void exd_query_exclude(exd_query_t * q, size_t component_index)
{
	assert(component_index < q->world->current_max_components);
	exd_entset_not(&q->matcher, &q->world->component_arrays[component_index].in_use_components);
}

void exd_query_merge_optional_group(exd_query_t * q, exd_query_optional_group_t * optionals)
{
	assert(optionals->optionals_count > 1);
	exd_entset_and(&q->matcher, &optionals->matcher);
}

exd_query_iter_t exd_query_compile(exd_query_t * q)
{
	return (exd_query_iter_t){
		.q = q,
		.current_index = 0
	};
}

//====================================
//     Optional component queries
//====================================

void exd_query_optional_group_init(exd_query_optional_group_t * optionals, exd_world_t * world, allocator_t * mem)
{
	exd_entset_init(&optionals->matcher, mem);
	optionals->world = world;
	optionals->optionals_count = 0;
}

void exd_query_optional_group_push(exd_query_optional_group_t * optionals, size_t component_index)
{
	assert(component_index < optionals->world->current_max_components);
	assert(optionals->optionals_count < EXD_QUERY_MAX_OPTIONAL_COMPONENT_MERGE);

	optionals->optionals[optionals->optionals_count] = &optionals->world->component_arrays[component_index].in_use_components;
	optionals->optionals_count++;
}

void exd_query_optional_group_compile(exd_query_optional_group_t * optionals)
{
	//It only makes sense to have an optional group if we're combining at least two component bitsets
	assert(optionals->optionals_count > 1);

	exd_entset_copy_data_from(&optionals->matcher, optionals->optionals[0]);

	//Note: Skip index 0, as we're using that as the base entset for matching already
	for (size_t i = 1; i < optionals->optionals_count; ++i)
	{
		exd_entset_or(&optionals->matcher, optionals->optionals[i]);
	}
}



//====================================
//          Entity iter ops
//====================================

//TODO(zshoals): For debug mode, this version of the iterator may be too slow.
//We need to find a way to shortcircuit the iteration harder so that we don't have to
//scan over so many slots while iterating.
exd_iterable_entity_t exd_query_iter_next(exd_query_iter_t * it)
{
	exd_entset_t * iteration_list = &it->q->matcher;

	//Note: Scary looking, but safe. This skip will only occur at bitflags offset 0 in a particular
	//bitset block. If any bit is set in the block, this just gets repeatedly skipped until the next block
	//Decent speedup results for sparse arrays
	while(exd_entset_entity_block_is_empty(iteration_list, it->current_index) && it->current_index < EXD_MAX_ENTITIES)
	{
		it->current_index += EXD_ENTSET_BITWIDTH;
	}

	while(exd_entset_slot_is_not_set(iteration_list, it->current_index) && it->current_index < EXD_MAX_ENTITIES)
	{
		it->current_index++;
	}

	u16 enabled_entity_slot = it->current_index;
	it->current_index++;

	return (exd_iterable_entity_t){
		.id = exd_entity_list_resolve_index(&it->q->world->entities, enabled_entity_slot)
	};
}
