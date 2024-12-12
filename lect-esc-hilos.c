
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_L 3
#define MAX_E 2

int dato = 0;

void *lector(void *arg) {
    int id = *(int *)arg;
    while (1) {
        // Leer datos
        printf("Lector %d leyendo: %d\n", id, dato);
        // Retraso aleatorio de hasta 1 milisegundo
        usleep(rand() % 1000);
    }
    return NULL;
}

void *escritor(void *arg) {
    int id = *(int *)arg;
    while (1) {
        // Escribir datos
        int aux = dato;
        usleep(rand() % 1000); // Introducir retraso
        aux++;
        dato = aux;
        printf("Escritor %d escribiendo: %d\n", id, dato);
        // Retraso aleatorio de hasta 1 milisegundo
        usleep(rand() % 1000);
    }
    return NULL;
}

int main(void) {
    // MAX_L lectores y MAX_E escritores
    pthread_t lectores[MAX_L], escritores[MAX_E];
    int i;
    int ids[MAX_L + MAX_E];

    // Inicializar la semilla del generador de números aleatorios
    srand(time(NULL));

    // Crear lectores
    for (i = 0; i < MAX_L; i++) {
        ids[i] = i;
        pthread_create(&lectores[i], NULL, lector, &ids[i]);
    }

    // Crear escritores
    for (i = 0; i < MAX_E; i++) {
        ids[MAX_L + i] = i;
        pthread_create(&escritores[i], NULL, escritor, &ids[MAX_L + i]);
    }

    // Esperar a que los hilos terminen
    for (i = 0; i < MAX_L; i++) {
        pthread_join(lectores[i], NULL);
    }
    for (i = 0; i < MAX_E; i++) {
        pthread_join(escritores[i], NULL);
    }

    printf("Acaba el main\n");
    return 0;
}