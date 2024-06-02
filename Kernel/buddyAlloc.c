#include "./include/memory_manager.h"

#ifdef BUDDY

#define PAGE_SIZE 4096 // (4KB)

typedef struct block {
    struct block *next;
    size_t size;
}block_t;

static block_t** free_blocks;
static uint8_t *base_memory;
static size_t max_memory_size;
static int max_order;

/* blocks in freelists[i] are of size 2**(i + 2) * PAGE_SIZE */
#define BLOCKSIZE(i) ((size_t)(1 << (i + 2)) * PAGE_SIZE)

/* the address of the buddy of a block from freelists[i] */
#define BUDDYOF(b, i) ((block_t*)((uintptr_t)(b) ^ (BLOCKSIZE(i))))


int log2(size_t n) {
    int log = 0;
    while (n >>= 1)
        log++;
    return log;
}

size_t pow2(int n) {
    return (size_t)1 << n;
}


void mm_init(void * ptr, size_t max_size) {
    base_memory = (uint8_t*) ptr;
    max_memory_size = max_size;

    max_order = log2(max_size/PAGE_SIZE) - 2;

    size_t free_blocks_size = (max_order + 1) * sizeof(block_t*);
    free_blocks = (block_t**)base_memory;
    base_memory += free_blocks_size;

    uintptr_t addr = (uintptr_t)base_memory;
    addr = ((addr + (PAGE_SIZE - 1)) / PAGE_SIZE) * PAGE_SIZE;
    base_memory = (uint8_t *)addr;

    for(int i= 0; i <= max_order; i++){
        free_blocks[i] = NULL;
    }

    int initial_order = max_order;
    size_t block_size = BLOCKSIZE(initial_order);

    block_t * initial_block = (block_t *)base_memory;
    free_blocks[initial_order] = initial_block;
    initial_block->next = NULL;
    initial_block->size = block_size;
}

void* mm_alloc(size_t size) {
    int i = 0;

    while(BLOCKSIZE(i) < size + sizeof(block_t) && i <= max_order){
        i++;
    }

    if(i > max_order){
        return NULL;
    } else if (free_blocks[i] != NULL) {
        block_t* block = free_blocks[i];
        free_blocks[i] = free_blocks[i]->next;
        block->size = BLOCKSIZE(i);
        return (void*)(block);
    } else {
        block_t* block = (block_t*)mm_alloc(BLOCKSIZE(i + 1) - sizeof(block_t));
        if (block != NULL) {
            block = (block_t*)((uintptr_t)block - (uintptr_t)base_memory); // reative position

            block_t* buddy = BUDDYOF(block, i);
            buddy = (block_t*)((uintptr_t)buddy + (uintptr_t)base_memory); // reverse to absolute after getting the buddy

            buddy->size = BLOCKSIZE(i);
            buddy->next = free_blocks[i];

            free_blocks[i] = buddy;
            block = (block_t*)((uintptr_t)block + (uintptr_t)base_memory);  // reverse to absolute after getting the buddy
            block->size = BLOCKSIZE(i);
            return (void*)(block);
        }
        return NULL;
    }
}

void mm_free(void * ptr) {
    block_t *block = (block_t *)ptr;
    int i = 0;
    while(BLOCKSIZE(i) < block->size && i <= max_order){
        i++;
    }

    if(i > max_order){
        return;
    }
    block = (block_t*)((uintptr_t)block - (uintptr_t)base_memory);
    block_t* buddy = BUDDYOF(block, i);
    block = (block_t*)((uintptr_t)block + (uintptr_t)base_memory);
    buddy = (block_t*)((uintptr_t)buddy + (uintptr_t)base_memory);
    block_t** p = &free_blocks[i];

    //verifico que no se encuetre en la lisat de bloques libres
    while ((*p != NULL) && (*p != buddy)) {
        p = &(*p)->next;
    }

    if(*p != buddy){
        block->next = free_blocks[i];
        free_blocks[i] = block;
    }else {
        // el buddy se encuntra libre, fusionamos los bloques
        *p = buddy->next;

        if(block > buddy){
            mm_free(buddy);
        }else{
            mm_free(block);
        }
    }
}

#endif