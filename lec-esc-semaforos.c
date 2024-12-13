#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h> //para utilizar intptr_t: convertir punteros a enteros del mismo tamaño

#define MAX_L  3
#define MAX_E  2

sem_t mutex;
sem_t m_escritores;

int dato = 0;
int nlectores = 0;

void *lector(void *arg) {
     int *id = (int*)arg;//SOLUCIÓN AL WARNING
   while(1){
        // Leer datos
	sem_wait(&mutex);//Para acceder a la variable compartida después de que hayan escrito
	nlectores++; //Comprobar que lectores es igual a 0?
        printf("Lector %d leyendo: %d\n", *id, dato);
	nlectores--;
	sem_post(&mutex); //Damos acceso a la variable al resto de lectores

        usleep(rand() % 1000000); // Retraso aleatorio de hasta 1 segundo (en microsegundos)
   }
}

void *escritor(void *arg) {
    int *id = (int*)arg;
    // Escribir datos y forzar la condición de carrera añadiendo instrucciones y aumentandoel numero de esperas aleatorias
    while(1){
	int sval;//Valor del semáforo
	
	sem_wait(&m_escritores);//comprobamos que no haya otro escritor escribiendo
	//Comprobar que no hay lectores leyendo
	if(nlectores==0){//Yo pondría un semáforo para controlar el acceso a la variable nlectores
        	int aux = dato;
                usleep(rand() % 200); // Introducir retraso
                aux++;
                usleep(rand() % 200);
                dato = aux;
		printf("Escritor %d escribiendo: %d\n", *id, dato);	
		sem_getvalue(&m_escritores, &sval);
		printf("Escritores esperando: %d\n", sval);
		sem_post(&mutex);//Ahora los lectores pueden acceder
		sem_post(&m_escritores);//Ahora otros escritores pueden intentar escribir
	}
        usleep(rand() % 1000000); // Retraso aleatorio de hasta 1 segundo (en microsegundos)
    }
}

int main() {
    //MAX_L Lectores y MAX_E escritores
    pthread_t lectores[MAX_L], escritores[MAX_E];
    int i;
    sem_init(&mutex, 0, 0);//Queremos que los lectores no lean hasta que hayan escrito por eso iniciamos a 0
    sem_init(&m_escritores, 0, 1);//Es para exclusión mutua, por eso se inicia a 1

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
	//usar la estructura atrib creada con pthread_attr_init atrib, da error: to_many_arguments, así que usamos NULL
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


