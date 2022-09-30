#pragma once

//Note: EXD_MAX_ENTITIES should be a power of 2 value between 32 and 65536
//This is enforced by crashing, probably, if you don't set this correctly.
#define EXD_MAX_ENTITIES (1 << 13)
#define EXD_MAX_COMPONENTS 128

#define EXD_QUERY_MAX_OPTIONAL_COMPONENT_MERGE 4
