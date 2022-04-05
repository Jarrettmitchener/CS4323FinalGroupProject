#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_OF_SOFAS 7
#define WAITING_ROOM_SIZE 9

typedef struct _thread_data_t
{
    int tid;
    double stuff;
} thread_data_t;

pthread_mutex_t lock[10];
pthread_mutex_t sofaLock;
sem_t roomLock;

int flag[10];
int sofaCounter = 0;
int waitingroomCounter = 0;

void initializeMutex()
{
    for(int i = 0; i < 10; i++)
    {
        pthread_mutex_init(&lock[i], NULL);
        pthread_mutex_lock(&lock[i]);
    }
}

void arrInit()
{
    for(int i = 0; i < 10; i++)
    {
        flag[i] = 0;
    }
}

//adds person to sofa
int addPersonToSofa()
{
    if(sofaCounter == NUM_OF_SOFAS)
    {
        return 0;
    }
    else
    {
        sofaCounter++;
        return 1;
    }
}

int addPersonToWaitingRoom()
{
    if(waitingroomCounter == WAITING_ROOM_SIZE)
    {
        return 0;
    }
    else
    {
        waitingroomCounter++;
        return 1;
    }
}
/* thread function */
void *thr_func(void *arg)
{
    while(1)
    {
        //converts arg to thread data object
        thread_data_t *data = (thread_data_t *)arg;
        int id = data->tid;
        while(flag[id] == 0)//gets stuck until flag is set to something else
        {
            //usleep(5000000);
        }

        printf("hello from thr_func, thread id: %d\n", id);

        //critical section for the semaphore
        sem_wait(&roomLock);
        int r = addPersonToWaitingRoom();
        if(r == 0)
        {
            printf("thread ID: %i could not be added to waiting room\n", id);
        }
        else
        {
            printf("thread ID: %i could be added to waiting room\n", id);
        }
        sem_post(&roomLock);

        //critical section begin
        pthread_mutex_lock(&sofaLock);
        int temp = addPersonToSofa();
        if(temp == 0)
        {
            printf("thread ID: %i could not be added to sofas\n\n", id);
        }
        else
        {
            printf("thread ID: %i could be added to sofas\n\n", id);
        }
        pthread_mutex_unlock(&sofaLock);

        flag[id] = 0;//resets flag
        pthread_exit(NULL);
    }
 
}

int main(int argc, char **argv)
{
    initializeMutex();
    arrInit();
    
    pthread_t thr[10];
    int i, rc;
    pthread_mutex_init(&sofaLock, NULL);
    sem_init(&roomLock, 0, 3);//initializes semaphore to 3
    /* create a thread_data_t argument array */
    thread_data_t thr_data[10];
    for (i = 0; i < 10; ++i) {
        thr_data[i].tid = i;
        thr_data[i].stuff = (i + 1) * 10;
        if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i])))
        {
            //this will only run if the thread cannot be created 
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    srand(time(NULL));
    sleep(1);
    for(int i = 0; i < 10; i++)
    {
        
        int t = rand() % 5;
        printf("time value: %i, iteration: %i\n", t, i);
        flag[i] = 1;
        sleep(2);
    }
}