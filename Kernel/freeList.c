#include "include/memory_manager.h"

//cada zona libre u ocupada tiene un header al pricnipio, que contiene
//un puntero al siguiente bloque, al bloque previo y el tamaño de toda la zona de memoria

typedef struct Node {
    Node *prev;
    unsigned size;
    Node *next;
} Node;

static Node * first = NULL;

void slab_init(void * ptr, size_t max_size) {
    first = (Node *) ptr;
    first->next = first->prev = NULL;
    first->size = max_size/sizeof(Node); //esto serviria de align dejando N nodes para manejar
}

void * slab_alloc(size_t n_bytes) {
    Node * ptrNode = first;
    n_nodes = (n_bytes + sizeof(Node) - 1)/sizeof(Node) + 1
    while (ptrNode != NULL) {
        if (ptrNode->size >= n_nodes) {
            if (ptrNode->size == n_nodes) {
                if (ptrNode == first)
                    first = ptrNode->next;
                else
                    (ptrNode->prev)->next = ptrNode->next;
            }
            else {
                ptrNode->size -= n_nodes;
                ptrNode += ptrNode->size;
                ptrNode->size = n_nodes;
                //reserva lugar desde el final del espacio libre y le agrega el header correspondiente
            }
            return (void *) (p + 1); //saltea el nodo header
        }
        ptrNode = ptrNode->next;
    }
    return NULL; //No hay memoria que entregar
}

void slab_free(void* ptr) {
    Node * ptrNode = (Node *)ptr - 1; //accede al header de la zona de memoria. El next y prev de la zona ocupada no tiene informacion valida

    if(first==NULL || first >= ptrNode){ //bloque al principio
        ptrNode->next = first;
        ptrNode->prev = NULL;
        first = ptrNode;
        return;
    }

    Node * p = first;
    while( !(ptrNode > p && ptrNode < p->next) ) //busco los bloques de memoria libre que encierran el bloque de memoria usada
        if(p->next == NULL)
            break; //me quedo con el ultimo nodo

    if(p->next==NULL){ //bloque al final
        ptrNode->next=NULL;
        ptrNode->prev=p;
        p->next = ptrNode;
        return;
    }

    if(ptrNode + ptrNode->size == p->next) {
        ptrNode->size += p->next->size;
        ptrNode->next = p->next->next;
    }else{
        ptrNode->next=ptr;
    }

    if(ptrNode == p + p->size){
        p->size += ptrNode->size;
        p->next = ptrNode->next;
    }else{
        p->next = ptrNode;
    }
}