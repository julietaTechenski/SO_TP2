#include "memory_manager.h"


#define PAGE_SIZE 4096 // (4KB)
#define MAX_PAGES 32768
#define MAX_ORDER 14 // Niveles: 64, 32, 16, 8 y 4 paginas

typedef struct block {
    struct block *next;
}block_t;

static uint8_t * base_ptr;
static block_t* free_blocks[MAX_ORDER];

void buddy_init(uint8_t * ptr) {
    base_ptr = ptr;

    for(int i = 0; i < MAX_ORDER; i++)
        free_blocks[i] = NULL;

    free_blocks[MAX_ORDER-1] = (block_t *)ptr;
    free_blocks[MAX_ORDER-1]->next = NULL;
}

void* buddy_alloc(size_t size) {
    int order = 0;   // 0 = 4 paginas
    size_t block_size = 4*PAGE_SIZE;

    // averiguo tamaño de bloque
    while(block_size < size){
        block_size = block_size*2;
        order++;

        if(order > MAX_ORDER){  // si la solicitud es de mas de MAX_PAGES , devuelvo null
            return NULL;
        }
    }

    int current_order = order;
    while(current_order < MAX_ORDER && free_blocks[current_order] == NULL){
        current_order++;
    }

    if(current_order >= MAX_ORDER)   // no hya bloques disponibles de ningun tamaño, todso los bloques tinene un tamaño mas pequeño o solicitud mayor a MAX_ORDER
        return NULL;

    while (current_order > order){
        block_t * aux_block = free_blocks[current_order];
        free_blocks[current_order] = aux_block->next;  // acutualizo bloques libres
        current_order--;

        block_t * buddy = (block_t*)((uint8_t *)aux_block + (block_size / (1 << current_order)));   // (1<< current_order) equivalente a elevar 2^(current order)  -  buddy: dirección de bloque resultante de dividir el bloque mas grande a la mitad del tamaño
        buddy->next = free_blocks[current_order]; // asignacion de bloque disponible
        free_blocks[current_order] = buddy;
    }

    block_t * reserved = free_blocks[order];
    free_blocks[order] = reserved->next;
    return (void*)reserved;
}

void buddy_free(void * ptr) {
    size_t offset = ((uint8_t*)ptr) - base_ptr;
    int order = 0;

    while(offset % ((MAX_PAGES / (1 << order)) * PAGE_SIZE) == 0 && order < MAX_ORDER){
        order++;
    }

    block_t * block = (block_t*)ptr;

    size_t buddy_offset = offset ^ ((MAX_PAGES / (1 << order)) * PAGE_SIZE);
    block_t * buddy = (block_t*) (base_ptr + buddy_offset);

    block_t** current = &free_blocks[order];
    while (*current != NULL) {
        if (*current == buddy) {
            *current = buddy->next;
            if (buddy_offset < offset) {
                block = buddy;
            }
            offset = (uint8_t*)block - base_ptr;
            order++;
            buddy_offset = offset ^ ((MAX_PAGES / (1 << order)) * PAGE_SIZE);
            buddy = (block_t*)(base_ptr + buddy_offset);
            current = &free_blocks[order];
        } else {
            current = &(*current)->next;
        }
    }

    block->next = free_blocks[order];
    free_blocks[order] = block;
}