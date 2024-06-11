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
    Node *nodePtr = (Node *) ptr - 1;
    total += nodePtr->size;

    if (nodePtr <= first) {
        nodePtr->next = first;
        first = nodePtr;
        return;
    }

    Node *iterPtr = first;
    while (iterPtr != NULL) {
        Node * endOfIter = (Node *)((size_t) iterPtr + iterPtr->size + sizeof(Node));
        Node * endOfNode = (Node *)((size_t) nodePtr + nodePtr->size + sizeof(Node));

        if (iterPtr->next == NULL) {
            if (endOfIter == nodePtr) {
                iterPtr->size += nodePtr->size + sizeof(Node);
            } else {
                iterPtr->next = nodePtr;
                nodePtr->next = NULL;
            }
            return;
        }

        if (iterPtr->next >= nodePtr) {

            if (endOfIter == nodePtr) {
                iterPtr->size += nodePtr->size + sizeof(Node);

                if (endOfIter == iterPtr->next) {
                    iterPtr->size += iterPtr->next->size + sizeof(Node);
                    iterPtr->next = iterPtr->next->next;
                }
                return;
            }

            if (endOfNode == iterPtr->next) {
                nodePtr->size += iterPtr->next->size + sizeof(Node);
                nodePtr->next = iterPtr->next->next;
                iterPtr->next = nodePtr;
                return;
            }

            nodePtr->next = iterPtr->next;
            iterPtr->next = nodePtr;
            return;
        }

        iterPtr = iterPtr->next;
    }
}


void mm_state() {
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