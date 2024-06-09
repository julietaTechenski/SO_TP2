#include "../include/memory_manager.h"
#ifdef LIST

typedef struct Node {
    struct Node * prev;
    unsigned size;
    struct Node *next;
} Node;

static Node * first = NULL;
static int freeMem;
static int total;

void mm_init(void * ptr, size_t max_size) {
    first = (Node *) ptr;
    total = freeMem = max_size;
    first->next = first->prev = NULL;
    first->size = max_size/sizeof(Node); //esto serviria de align dejando N nodes para manejar
}

void * mm_alloc(size_t n_bytes) {
    Node * ptrNode = first;
    int n_nodes = (n_bytes + sizeof(Node) - 1)/sizeof(Node) + 1;
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
            freeMem -= n_bytes;
            return (void *) (ptrNode + 1); //saltea el nodo header
        }
        ptrNode = ptrNode->next;
    }
    return NULL; //No hay memoria que entregar
}

void mm_free(void* ptr) {
    Node * ptrNode = (Node *)ptr - 1; //accede al header de la zona de memoria. El next y prev de la zona ocupada no tiene informacion valida

    freeMem += ptrNode->size;

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

void mm_state(){
    char s[20];
    int longitud;

    longitud = intToString(total, s);
    writeString(1, "Memoria total: ", 15);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);


    longitud = intToString(freeMem, s);
    writeString(1, "Memoria libre: ", 15);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(total - freeMem, s);
    writeString(1, "Memoria ocupada: ", 17);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

}

#endif