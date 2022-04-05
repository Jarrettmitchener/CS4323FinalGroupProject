#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct _thread_data_t
{
    int tid;
    double stuff;
} thread_data_t;

pthread_mutex_t lock[10];
int flag[10];

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

/* thread function */
void *thr_func(void *arg)
{
    while(1)
    {
        //converts arg to thread data object
        thread_data_t *data = (thread_data_t *)arg;
        
        while(flag[data->tid] == 0)//gets stuck until flag is set to something else
        {
            //usleep(5000000);
        }

        printf("hello from thr_func, thread id: %d\n", data->tid);
        flag[data->tid] = 0;//resets flag
        pthread_exit(NULL);
    }
 
}

int main(int argc, char **argv)
{
    initializeMutex();
    arrInit();
    

    pthread_t thr[10];
    int i, rc;
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

    for(int i = 0; i < 10; i++)
    {
        sleep(2);
        flag[i] = 1;
    }
}