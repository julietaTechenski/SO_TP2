// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/time.h"
#include "../include/process.h"

// Estructura para un nodo en la cola
typedef struct Node {
    int pid;              // ID del proceso
    unsigned int time_ms; // Tiempo en milisegundos para dormir
    struct Node *next;    // Puntero al siguiente nodo
} Node;

Node * head = NULL;
int ticks = 0;

// Handler para el temporizador
void timer_handler() {
    ticks++;

    Node *current = head;

    while (current != NULL && current->time_ms <= ticks) {
        // Despertar proceso
        unblock(current->pid);
        // Eliminar nodo de la lista
        head = current->next;
        mm_free(current);

        current = head;
    }
}

// Función para agregar un proceso a la cola de espera
void sleep(unsigned int ms) {
    Node *new_node = (Node *) mm_alloc(sizeof(Node));


    new_node->pid = getPID();
    new_node->time_ms = ticks + ms/55;

    // Insertar el nodo en la lista ordenada por `time_ms`
    if (head == NULL || head->time_ms > new_node->time_ms) {
        new_node->next = head;
        head = new_node;
    } else {
        Node *current = head;
        while (current->next != NULL && current->next->time_ms <= new_node->time_ms) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    block(new_node->pid);
}

// Función para obtener el número actual de ticks
int ticks_elapsed() {
    return ticks;
}

// Función para obtener los segundos transcurridos desde el inicio
int seconds_elapsed() {
    return ticks / 18; // Ejemplo de conversión de ticks a segundos
}
