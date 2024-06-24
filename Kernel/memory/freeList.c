// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/memory_manager.h"
#ifdef LIST


typedef struct Node {
    size_t size;
    struct Node * next;
} Node;

static Node * resize(Node * ptr, size_t size);

static Node * first = NULL;

static size_t total, freeMem;

void mm_init(void * ptr, size_t max_size) {
    first = (Node *) ptr;
    first->next = NULL;
    first->size = total = freeMem = max_size;
}

void * mm_alloc(size_t size) {
    if (first == NULL)
        return NULL;

    if (first->size >= size) {
        freeMem-=size;
        if (first->size == size) {
            Node * aux = first + 1;
            first = first->next;
            return aux;
        } else {
            return resize(first, size);
        }
    }

    Node * ptr = first;
    while (ptr->next != NULL) {
        if (ptr->next->size >= size) {
            freeMem -= size;
            Node * aux = ptr->next;
            if (ptr->next->size == size) {
                ptr->next = ptr->next->next;
                return aux + 1;
            } else
                return resize(aux, size);
        } else {
            ptr = ptr->next;
        }
    }
    return NULL;
}

static Node * resize(Node * ptr, size_t size) {
    ptr->size -= (size + sizeof(Node));
    Node * newHeader = (Node *) ( (size_t)ptr + ptr->size + sizeof(Node) );
    newHeader->next = NULL;
    newHeader->size = size;
    return newHeader + 1;
}


void mm_free(void *ptr) {
    Node *nodePtr = (Node *) ptr - 1;  // Limite inferior
    Node *endOfNode = (Node *) ((size_t) nodePtr + nodePtr->size + sizeof(Node));  // Limite superior
    freeMem += nodePtr->size;
    if (endOfNode <= first) {  // Va al principio
        if (endOfNode == first) {
            nodePtr->next = first->next;
            nodePtr->size += first->size + sizeof(Node);
            first = nodePtr;
        } else {
            nodePtr->next = first;
            first = nodePtr;
        }
        return;
    }

    Node *iterPtr = first;
    while (iterPtr != NULL) {
        if (iterPtr->next == NULL) {  // Va al final
            if (((size_t) iterPtr) + iterPtr->size + sizeof(Node) == (size_t) nodePtr) {
                iterPtr->size += nodePtr->size + sizeof(Node);
            } else {
                iterPtr->next = nodePtr;
                nodePtr->next = NULL;
            }
            return;
        }

        if (iterPtr->next >= nodePtr) {  // Va antes del siguiente bloque

            if (((size_t) iterPtr) + iterPtr->size + sizeof(Node) == (size_t) nodePtr) {  // Junto bloque inferior
                iterPtr->size += nodePtr->size + sizeof(Node);
                if (((size_t) iterPtr) + iterPtr->size + sizeof(Node) == (size_t) iterPtr->next) {  // Check bloque superior también
                    iterPtr->size += iterPtr->next->size + sizeof(Node);
                    iterPtr->next = iterPtr->next->next;
                }
                return;
            }

            if ((Node *) (((size_t) nodePtr) + nodePtr->size + sizeof(Node)) == iterPtr->next) {  // Junto SOLO con bloque superior
                nodePtr->size += iterPtr->next->size + sizeof(Node);
                nodePtr->next = iterPtr->next->next;
                iterPtr->next = nodePtr;
                return;
            }

            // No junto con nada
            nodePtr->next = iterPtr->next;
            iterPtr->next = nodePtr;
            return;
        }

        iterPtr = iterPtr->next;
    }

    // ERROR
}

void mm_state() {
    char s[20];
    int longitud;

    longitud = intToString(total, s);
    write(1, "Total memory: ",14);
    write(1, s, longitud);
    write(1, "\n", 1);


    longitud = intToString(freeMem, s);
    write(1, "Free memory: ", 13);
    write(1, s, longitud);
    write(1, "\n", 1);

    longitud = intToString(total - freeMem, s);
    write(1, "Allocated memory: ", 18);
    write(1, s, longitud);
    write(1, "\n", 1);

}

#endif