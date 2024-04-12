/*-------------------------------------------------------------

* Programación Avanzada: Manejo de Semáforos.

* Autor: Miguel Marines

* Actividad: IPC

*------------------------------------------------------------*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
 
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N


// Arreglos 
int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
 
// Semáforos
sem_t mutex;
sem_t S[N];
 
// Prueba el estado de los filósofos.
void test(int phnum)
{
    // Verifica el estádo de los filósofos
    if ( state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        // Cambia el estado a comiendo.
        state[phnum] = EATING;
 
        sleep(2);
 
        printf("Philosopher %d takes fork %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
 
        printf("Philosopher %d is Eating\n", phnum + 1);
 
        sem_post(&S[phnum]);
    }
}
 
// Toma el tenedor.
void take_fork(int phnum)
{
 
    // Bloquea el semáforo.
    sem_wait(&mutex);
 
    // Cambia el estado a hambriento.
    state[phnum] = HUNGRY;
 
    printf("Philosopher %d is Hungry\n", phnum + 1);
 
    // Come si los filosofos vecinos no están comiendo.
    test(phnum);
 
    sem_post(&mutex);
 
    // Si no puede comer espera su turno.
    sem_wait(&S[phnum]);
 
    sleep(1);
}
 
// Deja el tenedor
void put_fork(int phnum)
{
 
    // Bloque el semáforo.
    sem_wait(&mutex);
 
    // Cambia el estado a pensante.
    state[phnum] = THINKING;
 
    printf("Philosopher %d putting fork %d and %d down\n", phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);
 
    test(LEFT);
    test(RIGHT);
 
    // Libera el semáforo.
    sem_post(&mutex);
}
 
void* philospher(void* num)
{
    int j = 10;
 
    while (j-- > 0)
    {
        int* i = num;
 
        sleep(1);
 
        // Toma el teneedor.
        take_fork(*i);
 
        sleep(0);
 
        // Deja el teneedor.
        put_fork(*i);
    }
}
 
int main()
{
 
 	// Variable para controlar los ciclos for.
    int i;


    pthread_t thread_id[N];
 
    // Inicializa los semáforos.
    sem_init(&mutex, 0, 1);
 
    // El ciclo se ejecuta N (5) veces.
    for (i = 0; i < N; i++)
    {
        // Inicializa los semáforos.
        sem_init(&S[i], 0, 0);
    }
 
    // El ciclo se ejecuta N (5) veces.
    for (i = 0; i < N; i++)
    {
        // Crea los procesos filósofos.
        pthread_create(&thread_id[i], NULL, philospher, &phil[i]);
 
        printf("Philosopher %d is thinking\n", i + 1);
    }
 

    // El ciclo se ejecuta N (5) veces.
    for (i = 0; i < N; i++)
    {
        // Detiene el hilo que la invoca hasta que un hilo determinado termina.
        pthread_join(thread_id[i], NULL);
        sem_destroy( &S[i] );
    }  

}