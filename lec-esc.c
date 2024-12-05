#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h> //para utilizar intptr_t: convertir punteros a enteros del mismo tamaño

#define MAX_L  3
#define MAX_E  2


int dato = 0;

void *lector(void *arg) {
    int id = (intptr_t)arg; //solución warning de cast de un puntero a un entero de tamaño diferente.
    while (1) {
       
        // Leer datos
        printf("Lector %d leyendo: %d\n", id, dato);
        usleep(rand() % 1000000); // Retraso aleatorio de hasta 1 segundo (en microsegundos)
    }
}

void *escritor(void *arg) {
    int id = (intptr_t)arg;
     int aux;
    while (1) {
        
        // Escribir datos
        dato ++;
        printf("Escritor %d escribiendo: %d\n", id, dato);

        usleep(rand() % 2000000); // Retraso aleatorio de hasta 2 segundos (en microsegundos)
    }
}

int main() {
    
/*
    // MAX_L lectores y MAX_E escritores
    pthread_t lectores[MAX_L], escritores[MAX_E]; 
    int i;
    pthread_attr_t atrib;

    // Inicializar la semilla del generador de números aleatorios
    srand(time(NULL)); 

    // Inicializar la estructura pthread_attr
   

    // Crear lectores
    

    // Crear escritores
    

    // Esperar a que los hilos terminen
    
    printf("Acaba el main\n");
    return 0;

    */

  pthread_t lectores[MAX_L], escritores[MAX_E];
    int i;

    // Inicializar la semilla del generador de números aleatorios
    srand(time(NULL));

    // Crear hilos lectores
    for (i = 0; i < MAX_L; i++) {
        int *arg = malloc(sizeof(*arg)); // Asignar memoria para cada argumento
        if (arg == NULL) {
            perror("Error al asignar memoria");
            exit(EXIT_FAILURE);
        }
        *arg = i; // Guardar el valor del índice en la memoria asignada
        if (pthread_create(&lectores[i], NULL, lector, arg) != 0) {
            perror("Error creando hilo lector");
            free(arg); // Liberar memoria en caso de error
            exit(EXIT_FAILURE);
        }
    }

    // Crear hilos escritores
    for (i = 0; i < MAX_E; i++) {
        int *arg = malloc(sizeof(*arg)); // Asignar memoria para cada argumento
        if (arg == NULL) {
            perror("Error al asignar memoria");
            exit(EXIT_FAILURE);
        }
        *arg = i; // Guardar el valor del índice en la memoria asignada
        if (pthread_create(&escritores[i], NULL, escritor, arg) != 0) {
            perror("Error creando hilo escritor");
            free(arg); // Liberar memoria en caso de error
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que terminen los hilos lectores
    for (i = 0; i < MAX_L; i++) {
        if (pthread_join(lectores[i], NULL) != 0) {
            perror("Error esperando hilo lector");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que terminen los hilos escritores
    for (i = 0; i < MAX_E; i++) {
        if (pthread_join(escritores[i], NULL) != 0) {
            perror("Error esperando hilo escritor");
            exit(EXIT_FAILURE);
        }
    }

    printf("Acaba el main\n");
    return 0;
}


