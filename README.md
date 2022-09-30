# exd_ecs_demo
Thanks for taking the time to read through this repository if you've clicked through! I would appreciate it if you would be willing to give me any feedback on this repository's organization, architecture, implementation, etc.

EXD is a bitset [entity-component-system](https://en.wikipedia.org/wiki/Entity_component_system) implementation.

Upfront: This "library" is really just for personal use. It's missing dependencies for logging and for memory allocation.

EXD's world is meant to be initialized once at startup, and then never freed, so there aren't any matching deinitialization functions for any of the containers. If something needs to be deleted, the allocator that the world was sourced from can have its memory reset instead.

It's intentionally pretty sparse on features. The entirety of the user facing API is in world.h and world_query.h.

Example usage:

```c
typedef enum
{
	COMP_POSITION,
	COMP_ROTATION,
	COMP_HEALTH,
	COMP_BURNING_STATUS_EFFECT,
	COMP_WEAPON_BASE,
	COMP_AUTO_PISTOL,

	COMP_MAX_COUNT
}
game_components_e;

typedef enum
{
	TAG_PLAYER = COMP_MAX_COUNT,
	TAG_ENEMY,
	TAG_FLAMMABLE,
	TAG_DAMAGEABLE,
	TAG_GOD_MODE,

	TAG_MAX_COUNT
}
game_tags_e;

//Note: Allocators aren't included, but you can pretend this is a malloc call instead
exd_world_t * world = allocator_malloc(default_permanent_allocator(), exd_world_t, 1);
exd_world_init(world, default_permanent_allocator());

exd_world_set_global_world_for_shorthand_access(world);

//Note: Struct typedefs (position_t, health_t, etc.) omitted for brevity
exd_world_component_create_component_storage(world, position_t, COMP_POSITION);
exd_world_component_create_component_storage(world, rotation_t, COMP_ROTATION);
exd_world_component_create_component_storage(world, health_t, COMP_HEALTH);
exd_world_component_create_component_storage(world, burn_status_t, COMP_BURNING_STATUS_EFFECT);
exd_world_component_create_component_storage(world, weapon_base_t, COMP_WEAPON_BASE);
exd_world_component_create_component_storage(world, auto_pistol_t, COMP_AUTO_PISTOL);

exd_world_component_create_tag_storage(world, TAG_PLAYER);
exd_world_component_create_tag_storage(world, TAG_ENEMY);
exd_world_component_create_tag_storage(world, TAG_FLAMMABLE);
exd_world_component_create_tag_storage(world, TAG_DAMAGEABLE);
exd_world_component_create_tag_storage(world, TAG_GOD_MODE);



//----------------------------------------
//That's the end of world initialization
//----------------------------------------



exd_entity_t player = exd_ent_new();
{
	position_t * player_pos = exd_comp_set(COMP_POSITION, player);
	player_pos->x = -50;
	player_pos->y = 75;

	health_t * player_health = exd_comp_set(COMP_HEALTH, player);
	player_health->health = 100;

	burn_status_t * player_burn = exd_comp_set(COMP_BURNING_STATUS_EFFECT, player);
	player_burn->per_tick_damage = 10;

	exd_comp_set(TAG_PLAYER, player);
	exd_comp_set(TAG_DAMAGEABLE, player);
	exd_comp_set(TAG_FLAMMABLE, player);
}


//----------------------------------------
//"Systems" that are just functions
//----------------------------------------

burn_system(world);
empty_health_kill_system(world);
//...more systems here, each executed sequentially


//----------------------------------------
//What systems look like in practice
//----------------------------------------

void burn_system(exd_world_t * world)
{
	exd_query_t q = {0};
	exd_query_init_from(&q, world, default_frame_allocator());
	{
		//Include entities with burning_status_effect, that are flammable, and have health
		exd_query_include(&q, COMP_BURNING_STATUS_EFFECT);
		exd_query_include(&q, TAG_FLAMMABLE);
		exd_query_include(&q, COMP_HEALTH);
	}
	exd_query_iter_t it = exd_query_compile(&q); 

	//&it matches our player from earlier, and anything else with these components and tags
	//"ent" is automatically created in this iter loop and only valid inside the loop. ent represents
	//a valid and guaranteed entity acquired via the iterator
	foreach_entity(ent, &it)
	{
		burn_status_t const * burn = exd_comp_get(COMP_BURNING_STATUS_EFFECT, ent);
		health_t * health = exd_comp_get_mut(COMP_HEALTH, ent);

		health->health -= burn->per_tick_damage;
	}
}

void empty_health_kill_system(exd_world_t * world);
{
	exd_query_t q = {0};
	exd_query_init_fast(&q, default_frame_allocator());
	{
		//Include entities with health,
		//but exclude any entities with a god mode state (aka invulnerable)

		exd_query_include(&q, COMP_HEALTH);

		exd_query_exclude(&q, TAG_GOD_MODE);
	}
	exd_query_iter_t it = exd_query_compile(&q);

	foreach_entity(ent, &it)
	{
		health_t const * health = exd_comp_get(COMP_HEALTH, ent);
		if (health->health <= 0)
		{
			exd_ent_kill(ent);
		}
	}
}
```

File breakdown:

* **exd-common.h**: Frequently used macros, typedefs, etc.
* **exd-math.h**: Power of 2 division and modulo
* **exd-config.h**: Configuration Defines intended to be changed by the user
* **exd-unit.c**: Manual unity build
* **bit.h**: Bitwise operations
* **entity.h**: Unique unsigned 64 bit integer Version/Handle combo
* **iterable_entity.h**: Special-cased entity that can skip some otherwise mandatory validation checks during query iteration
* **entset.h**: Bitsets accessed via entity handles
* **freelist.h**: Hands out unused entity IDs to the user, with some special cased design
* **entity_list.h**: "Master list" of entities, containing entity active/inactive states, all entity IDs and their current version/generation, and maintains an entity freelist
* **component_data.h**: Untyped component/data array storage, also doubles as "tag" storage (bitset-only components, no additional data)
* **world_query.h**: Bitset merging operations that generate an iterable list of entities matching a component search query
* **world.h**: Primary user interface, general storage and synchronization container for everything else