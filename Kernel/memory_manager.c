#include "memory_manager.h"

#ifdef BUDDY
#include "buddy.h"
    #define memory_manager_init buddy_init
    #define memory_manager_alloc buddy_alloc
    #define memory_manager_free buddy_free
#elif defined(SLAB)
#include "slab.h"
    #define memory_manager_init slab_init
    #define memory_manager_alloc slab_alloc
    #define memory_manager_free slab_free
#else
#error "No memory manager defined"
#endif