#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

//mutexes and semaphore variables declared here 
pthread_mutex_t waitingRoomMutex;
sem_t sofaSemaphore;


//counters used for functions
int sofaCounter = 0;
int waitingroomCounter = 0;

//variables that are used in testing
//will be removed later when program arguments are added;
int numOfSofas = 5;
int spaceOfWaitingRoom = 10;
int numOfThreads = 15;

//boolean array that will be used to determine when threads fire
int flag[15];

//struct for medical professional
typedef struct _medProf
{
    int id;
    double time;
} medProf;

//struct for patient
typedef struct _patient
{
    int id;
    double time;
} patient;

//function to add people to waiting room
int addWaitingRoom()
{
    if(waitingroomCounter == spaceOfWaitingRoom)
    {
        return 0;
    }
    else 
    {
        waitingroomCounter++;
        return 1;
    }
}
//function for medical professional threads
void* medProfFunc(void *arg)
{
    medProf *data = (medProf *)arg;
    int id = data->id;
}

//function for patient threads
void* patientFunc(void *arg)
{
    patient *data = (patient *)arg;
    int id = data->id;
    //printf("Thread ID %i has been created\n", id);
    

    //thread waits so it can only start when told to by main
    while(flag[id] == 0);
    //printf("Thread ID %i has started\n", id);


    //critical section for waiting room entering
    pthread_mutex_lock(&waitingRoomMutex);
    printf("Patient %3i (Thread ID %5i):Arriving at clinic\n", id, id);
    int leave = addWaitingRoom();
    if(leave == 0)
    {
        printf("Patient %3i (Thread ID %5i):Leaving clinic without checkup\n", id, id);
        //pthread_exit(NULL);//temp exit method because thread shouldn't do anything now
        pthread_mutex_unlock(&waitingRoomMutex);
    }
    else
    {
        printf("Patient %3i (Thread ID %5i):Going into the clinic\n", id, id);

        sleep(2);
        pthread_mutex_unlock(&waitingRoomMutex);
        //end of entering waiting room critical section

        //start of sofa semaphore critical section
        sem_wait(&sofaSemaphore);

        printf("Patient %3i (Thread ID %5i):Sitting on a sofa in the waiting room\n", id, id);
        sleep(2);
        sem_post(&sofaSemaphore);
        //end of sofa section
    }
    
}

void flag_init()
{
    for(int i = 0; i < numOfThreads; i++)
    {
        flag[i] = 0;
    }
}
int main()
{
    //intilization of semaphores, mutex locks, and flags
    sem_init(&sofaSemaphore, 0, numOfSofas);
    pthread_mutex_init(&waitingRoomMutex, NULL);
    flag_init();

    
    //creates the threadpool
    //currently only using patient implementation
    pthread_t threads[numOfThreads];
    patient patientData[numOfThreads];
    int rc;
    for(int i = 0; i < numOfThreads; i++)
    {
        //assigns patient data
        patientData[i].id = i;
        patientData[i].time = 0;
        if ((rc = pthread_create(&threads[i], NULL, patientFunc, &patientData[i])))
        {
            //this will only run if the thread cannot be created 
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }

    //activates the threads every second
    for(int i = 0; i < numOfThreads; i++)
    {
        sleep(1);
        flag[i] = 1;
    }

    //joins the threads so program waits on their activation
    int res;
    for (int i = 0; i < numOfThreads; ++i)
    {
        res = pthread_join(threads[i], NULL);
        //printf("%d\n", res);
    }
    printf("ALL HAVE BEEN JOINED\n");

}