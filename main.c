#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#include "Caleb.c"
#include "Keenan.c"
#include "Patrick.c"


//mutexes and semaphore variables declared here 
pthread_mutex_t waitingRoomMutex;
pthread_mutex_t checkSofa;
pthread_mutex_t checkWait;
pthread_mutex_t timing;
sem_t sofaSemaphore;


//counters used for functions
int waitingroomCounter = 0;

//statistic variables
int numOfSuccessfulCheckups = 0;
int numOfPatientsThatLeft = 0;
double avgWaitTimeForMedProf = 0;
double avgWaitTimeForPatients = 0;

//array used to store amount of time waiting to sit on sofa
int waitTimeArr[10000];
//amount of time sitting on sofa
int sofaWaitTimeArr[10000];

//patient and medical professionals status arrays
int patientArr[10000];
int medProfArr[10000];

//variables that are used in testing
//will be removed later when program arguments are added;
int spaceOfWaitingRoom = 10;
int numOfThreads = 10;
int numOfSofas = 5;

//boolean array that will be used to determine when threads fire
int flag[15];

//struct for medical professional
typedef struct _medProf
{
    int tid;
    int id;
    double waitTime;
    clock_t time;
} medProf;

//struct for patient
typedef struct _patient
{
    int tid;
    int id;
    double waitingRoomTime;
    double sofaWaitTime;
    clock_t time;
} patient;

//finds largest wait time array and returns if the
//specified thread is the one with the largest time;
int findMaxWaitTime(int id)
{
    int max = waitTimeArr[0];
    int maxIndex = 0;

    for(int i = 1; i < numOfThreads; i++)
    {
        if(waitTimeArr[i] > max)
        {
            max = waitTimeArr[i];
            maxIndex = i;
        }
    }
    if(getNumOfPatientsOnSofas() >= numOfSofas)
    {
        return 0;//means user doesnt get on sofa
    }
    else
    {
        if(maxIndex == id)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//finds largest wait time for sofa array and returns if the
//specified thread is the one with the largest time;
int findMaxSofaWaitTime(int PID)
{
    int max = sofaWaitTimeArr[0];
    int maxIndex = 0;

    for(int i = 1; i < numOfThreads; i++)
    {
        if(sofaWaitTimeArr[i] > max)
        {
            max = sofaWaitTimeArr[i];
            maxIndex = i;
        }
    }


    if(maxIndex == PID)
    {
        sofaWaitTimeArr[maxIndex] = -1;
        return 1;
    }
    else
    {
        return 0;
    }
}
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
//function to lower waiting room counter
//will be used when patient is being checked up on 
void removeWaitingRoom()
{
    waitingroomCounter--;
}

//adds number of Patients that left counter
void addPatientsLeft()
{
    numOfPatientsThatLeft++;
}

//setters for patient and medProf Arrays
void setPatientArr(int PID, int value)
{
    patientArr[PID] = value;
}

void setMedProfArr(int MID, int value)
{
    medProfArr[MID] = value;
}
//getters for patient and medical professional array
int getPatientStatus(int PID)
{
    return patientArr[PID];
}

int getMedProfStatus(int MID)
{
    return medProfArr[MID];
}
//start of functions that are needed according to document
//note that some of these functions really don't need to exist
//like sit on sofa, the semaphore already stops the sofa from 
//being sat on if the sofas are full
//because of this, I tried to give them some utility use
//and added more methods that would work with the utility use
//like adding to a global sofa counter variables


void enterWaitingRoom(int id, int tid)
{
    printf("Patient %3i (Thread ID %5i):Going into the clinic\n", id, tid);
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
    int tid = data->tid;

    //thread waits so it can only start when told to by main
    while(flag[id] == 0);

    //critical section for waiting room entering
    pthread_mutex_lock(&waitingRoomMutex);
    printf("Patient %3i (Thread ID %5i):Arriving at clinic\n", id, id);

    int leave = addWaitingRoom();
    if(leave == 0)
    {
        printf("Patient %3i (Thread ID %5i):Leaving clinic without checkup\n", id, id);

        //adds to amount of patients that didnt get a medical checkup
        addPatientsLeft();
        pthread_mutex_unlock(&waitingRoomMutex);
    }
    else
    {
        //sucessfully enters waiting room
        enterWaitingRoom(id, tid);
        data->time = clock();

        sleep(2);
        pthread_mutex_unlock(&waitingRoomMutex);
        //end of entering waiting room critical section

        int canSitOnSofa = 0;

        
        while(canSitOnSofa == 0)
        {
            clock_t waitClock = clock() - data->time;
            double timeTaken = ((double)waitClock)/CLOCKS_PER_SEC;
            waitTimeArr[id] = timeTaken;

            pthread_mutex_lock(&checkSofa);

            canSitOnSofa = findMaxWaitTime(id);
            if(canSitOnSofa == 1)
            {
                addToSofas();//keenan Holsapple
            }

            pthread_mutex_unlock(&checkSofa);
        }
        
        //start of sofa semaphore critical section
        sem_wait(&sofaSemaphore);

        sitOnSofa(id, tid);//keenan
        data->time = clock();
        
        //printf("Thread ID: %i took %f time\n", id, timeTaken);
        sleep(2);

        //temp gets off of sofa
        removeFromSofas();//keenan

        sem_post(&sofaSemaphore);
        //end of sofa section

        
        //section where it is determined if patient has longest wait time
        int canGetCheckup = 0;
        int counter = 0;
        while(canGetCheckup == 0)
        {
            clock_t sofaClock = clock() - data->time;
            double timeTaken = ((double)data->time)/CLOCKS_PER_SEC;
            sofaWaitTimeArr[id] = timeTaken;

            pthread_mutex_lock(&checkWait);

            canGetCheckup = findMaxSofaWaitTime(id);
            if(canGetCheckup == 1)
            {
                //addToSofas();//keenan Holsapple
                //printf()
            }

            pthread_mutex_unlock(&checkWait);
            if(counter > 500)
            {
                printf("Thread ID %i is BRUHHHHHing\n", id);
                sleep(2);
            }
            counter++;
        }

        printf("Thread ID %i has longest wait time on sofa\n", id);
        
    }
    
}

void flag_init()
{
    for(int i = 0; i < numOfThreads; i++)
    {
        flag[i] = 0;
    }
}

void statusInit()
{
    for(int i = 0; i < numOfThreads; i++)
    {
        patientArr[i] = -1;
        medProfArr[i] = -1;
    }
}
int main()
{
    //intilization of semaphores, mutex locks, and flags
    sem_init(&sofaSemaphore, 0, 5);
    pthread_mutex_init(&waitingRoomMutex, NULL);
    pthread_mutex_init(&checkSofa, NULL);
    pthread_mutex_init(&timing, NULL);
    flag_init();
    statusInit();

    //keenan
    setAmountOfSofas(numOfSofas);

    
    //creates the threadpool
    //currently only using patient implementation
    pthread_t threads[numOfThreads];
    patient patientData[numOfThreads];
    int rc;
    for(int i = 0; i < numOfThreads; i++)
    {
        //assigns patient data
        patientData[i].id = i;
        patientData[i].tid = i;
        patientData[i].waitingRoomTime = 0;

        waitTimeArr[i] = -1;

        if ((rc = pthread_create(&threads[i], NULL, patientFunc, &patientData[i])))
        {
            //this will only run if the thread cannot be created 
            fprintf(stderr, "error creating thread %i\n", i);
            return EXIT_FAILURE;
        }
        usleep(10);
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

    //destroys mutexes and semaphores
    sem_destroy(&sofaSemaphore);
    pthread_mutex_destroy(&waitingRoomMutex);

}