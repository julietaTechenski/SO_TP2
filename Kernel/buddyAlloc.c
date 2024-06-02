#ifdef BUDDY
#include "./include/memory_manager.h"
#define PAGE_SIZE 4096 // (4KB)
#define MAX_ORDER 5
#define sizes (MAX_ORDER + 1)

typedef struct block {
    struct block *next;
    size_t size;
}block_t;

static block_t* free_blocks[sizes];

/* blocks in freelists[i] are of size 2**(i + 2) * PAGE_SIZE */
#define BLOCKSIZE(i) ((size_t)(1 << (i + 2)) * PAGE_SIZE)

/* the address of the buddy of a block from freelists[i]. */
#define BUDDYOF(b, i) ((block_t*)((uintptr_t)(b) ^ (BLOCKSIZE(i))))

void error(const char *msg) {
    printf("Error: %s\n", msg);
}

void mm_init(uint8_t * ptr, size_t max_size) {
    for(int i = 0; i < sizes; i++)
        free_blocks[i] = NULL;

    free_blocks[MAX_ORDER] = (block_t *)ptr;
    free_blocks[MAX_ORDER]->next = NULL;
    initial_block->size = BLOCKSIZE(MAX_ORDER);
}

void* mm_alloc(size_t size) {
    int i = 0;

    while(BLOCKSIZE(i) < size){
        i++;
    }

    if (i >= sizes) {
        error("no space available");
        return NULL;
    }
    else if (free_blocks[i] != NULL) {
        // se encotro un bloque del tamaño que necesitamos
        block_t* block = free_blocks[i];
        free_blocks[i] = free_blocks[i]->next;
        block->size = BLOCKSIZE(i);
        return block;
    }
    else {
        block_t* block = allocate(BLOCKSIZE(i + 1));  //llamada recursiva para obtener un bloque mas grande
        if (block != NULL) {
            // Dividimos el bloque y ponemos el buddy en la lista de bloques libres
            block_t* buddy = BUDDYOF(block, i);
            buddy->size = BLOCKSIZE(i);
            buddy->next = free_blocks[i];
            free_blocks[i] = buddy;
            block->size = BLOCKSIZE(i);
        }
        return block;
    }
}

void mm_free(void * ptr) {
    size_t size = (block_t*)block->size;
    int i = 0;

    while(BLOCKSIZE(i) < size){
        i++;
    }
    block_t* buddy = BUDDYOF(block, i);
    block_t** p = &free_blocks[i];

    //verifico que no se encuetre en la lisat de bloques libres
    while ((*p != NULL) && (*p != buddy)) {
        p = &(*p)->next;
    }

    if(*p != buddy){
        block->next = free_blocks[i];
        free_blocks[i] = block;
    }else {
        // el buddy se encuntra ocupado, hay q librearlo
        *p = buddy->next;

        if(block > buddy)
            deallocate(buddy, BLOCKSIZE(i+1));
        else
            deallocate(block, BLOCKSIZE(i+1));
    }
}

#endif