/*----------------------------------------------------------------

* Programación Avanzada: Sincronización

* Autor: Miguel Marines

* Actiividad: (Producer - Consumer)

El problema se trata de un problema de sincronización
productor - consumidor, ya que Benji deja dos billetes en el
mostrador y los otakus los utilizan, por tanto Benji se convierte
en el productor de los billetes puestos en el mostrador y
los otakus en los consumidores.

*--------------------------------------------------------------*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>

#define N_CONSUMIDORES_OTAKUS 3
#define N_BILLETES_BENJI 2

sem_t mutex;
sem_t cond;

int i;

int buffer[N_BILLETES_BENJI];

int front = 0;
int rear = 0;
bool emptySpace = true;

pthread_t hilo_benji_productor;
pthread_t hilo_otaku_consumidor[N_CONSUMIDORES_OTAKUS];

void put(int value)
{
    buffer[front] = value;
    front = (front + 1) % N_BILLETES_BENJI;
}

int get()
{
    int tmp = buffer[rear]; 
    rear = (rear + 1) % N_BILLETES_BENJI;
    return tmp;
}

void *otaku_consumidor(void *arg)
{

  long id = (long) arg;

  while(true)
  {

    sem_wait(&mutex);

    while(emptySpace)
    {
      sem_post(&mutex);
      sem_wait(&cond);
      sem_wait(&mutex);
    }

    int position = front;
    for(i = 0; i < N_BILLETES_BENJI; i++)
    {
      get(i);
    }
    printf("El otaku %ld toma los billetes puestos por Benji en el mostrador y compra dulce japones Pocky.\n", id+1);

    if(position == (N_BILLETES_BENJI - 1))
    {
      emptySpace = true;
    }

    sem_post(&mutex);
    sleep(2);

  }

  return NULL;
}

void *benji_productor(void *arg)
{

  int i;
  
  while(true)
  {

    if(emptySpace)
    {

      sem_wait(&mutex);

      for(i = 0; i < N_BILLETES_BENJI; i++)
      {
        put(i);
      }
      printf("Benji deja 2 billetes en el mostrador.\n");
      
      emptySpace = false;
      
      sem_post(&cond);
      sem_post(&mutex);
    }

  }

  return NULL;
}


int main()
{
  sem_init(&mutex, 0, 1);
  sem_init(&cond, 0, 0);

  pthread_create(&hilo_benji_productor, NULL, benji_productor, NULL);

  for(long i = 0; i < N_CONSUMIDORES_OTAKUS; i++)
  {
    pthread_create(&hilo_otaku_consumidor[i], NULL, otaku_consumidor, (void *)i);
  }

  pthread_exit(NULL);
  
  return 0;
}