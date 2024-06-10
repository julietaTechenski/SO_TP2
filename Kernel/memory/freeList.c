// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/memory_manager.h"
#ifdef LIST

#include "stddef.h"

static unsigned mm_iner_free(void * ap);

// Estructura del bloque de memoria
typedef long Align;  // Para forzar la alineación en el límite de tipo long

union header {
    struct {
        union header *ptr;  // Puntero al siguiente bloque libre
        unsigned size;      // Tamaño del bloque
    } s;
    Align x;  // Para asegurar que el bloque esté correctamente alineado
};

typedef union header Header;

static Header base;             // Bloque base para inicialización
static Header *freep = NULL;    // Puntero a la lista libre

#define NALLOC 1024  // Número mínimo de unidades a solicitar

static void * ptrStart;
static size_t totalMem;
static size_t ocMem;

void mm_init(void * ptr, size_t max_size) {
    ptrStart = ptr;
    totalMem = max_size;
    ocMem = 0;
}

static Header *morecore(unsigned nu) {
    char *cp;
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;

    cp = ptrStart;
    ptrStart += (nu * sizeof(Header));

    up = (Header *) cp;
    up->s.size = nu;
    mm_iner_free((void *)(up + 1));
    return freep;
}

void * mm_alloc(size_t nbytes) {
    Header *p, *prevp;
    unsigned nunits;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL) {  // No hay lista libre inicial
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {  // Bloque suficientemente grande
            if (p->s.size == nunits)  // Exactamente
                prevp->s.ptr = p->s.ptr;
            else {  // Fragmentar
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            ocMem += (nunits * sizeof(Header));
            return (void *)(p + 1);
        }
        if (p == freep)  // Ha dado la vuelta completa
            if ((p = morecore(nunits)) == NULL)
                return NULL;  // No hay suficiente memoria
    }
}



static unsigned mm_iner_free(void * ap) {
    Header *bp, *p;

    bp = (Header *)ap - 1;  // Obtener puntero al bloque de cabecera
    unsigned aux = bp->s.size * sizeof(Header);
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;  // Liberar al final o inicio

    if (bp + bp->s.size == p->s.ptr) {  // Unir hacia adelante
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;

    if (p + p->s.size == bp) {  // Unir hacia atrás
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;

    freep = p;
    return aux;
}

void mm_free(void * ptr){
    unsigned aux = mm_iner_free(ptr);
    ocMem -= aux;
}

void mm_state() {
    char s[20];
    int longitud;

    longitud = intToString(totalMem, s);
    writeString(1, "Memoria total: ", 15);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);


    longitud = intToString(totalMem - ocMem, s);
    writeString(1, "Memoria libre: ", 15);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(ocMem, s);
    writeString(1, "Memoria ocupada: ", 17);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

}



#endif