#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_ALLOWED 2

sem_t mutex;

typedef struct _thread_data_t
{
    int tid;
    double stuff;
} thread_data_t;

void* threadFunc(void *arg)
{
    
    //sleep(1);
    thread_data_t *data = (thread_data_t *)arg;
    int id = data->tid;
    //printf("HI IM A THREAD ID %i\n", id);

    sem_wait(&mutex);

    sleep(2);
    printf("thread ID: %i has gotten through sem\n", id);
    

    sem_post(&mutex);
}

int main()
{
    sem_init(&mutex, 0, NUM_ALLOWED);

    pthread_t threads[10];
    thread_data_t thr_data[10];
    int i, rc;

    printf("about to make threads\n");
    for(int i = 0; i < 10; i++)
    {
        thr_data[i].tid = i;
        thr_data[i].stuff = (i + 1) * 10;
        //printf("f\n");
        if ((rc = pthread_create(&threads[i], NULL, threadFunc, &thr_data[i])))
        {
            //this will only run if the thread cannot be created 
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }

    }
    //sleep(10);

    int res;
    for (i = 0; i < 10; ++i)
    {
        res = pthread_join(threads[i], NULL);
        //printf("%d\n", res);
    }
    printf("ALL HAVE BEEN JOINED\n");
}