#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "header.h"

#include "caleb.c"
#include "keenan.c"
#include "patrick.c"
#include "utility.c"

//#include "test.c"

//temp variables that will be changed later
int numOfPatients = 10;
int numOfMedProfs = 1;
int numOfThreads;
int numOfSofas = 2;
int numOfWaitingRoomSlots = 7;

int sofaCounter = 0;

//arrays that will be used to determine the status of the patients/medical professionals
//-1: nothing has been done with them yet
int *patientStatusArr;
int *medProfStatusArr;

//keeps track of time for sofa and waiting room
double *waitingRoomTimeArr;
double *sofaWaitingTimeArr;
//thread activation array for patient threads
int* threadActivateArr;

//mutex that will be used for adding/removing waiting room 
pthread_mutex_t waitingRoomMutex;
pthread_mutex_t waitingRoomTimeMutex;
pthread_mutex_t checkSofaMutex;

//patient thread function
void* patientThread(void *arg)
{
    //gets patient data object
    patient *data = (patient *)arg;
    int id = data->id;
    int tid = data->tid;

    //thread waits so it can only start when told to by main
    while(threadActivateArr[id] == -1);

    pthread_mutex_lock(&waitingRoomMutex);

    //flag that determines if patient can enter the waiting room
    int canEnter = canEnterWaitingRoom(id, tid);

    //if they can enter the waiting room
    if(canEnter == 1)
    {

        enterWaitingRoom(id, tid);
        patientStatusArr[id] = 0;//updates patient status
        pthread_mutex_unlock(&waitingRoomMutex);

        double time = getTime();//used to get final time waited for sofa
        
        int canSitOnSofa = 0;
        //while loop so that it waits until user is the one with the most time in waiting room to sit on the sofa
        while(canSitOnSofa == 0)
        {
            pthread_mutex_lock(&checkSofaMutex);

            double tempTime = getTime() - time;
            waitingRoomTimeArr[id] = tempTime;

            canSitOnSofa = canMoveToSofa(id);
            if(canSitOnSofa == 1)
            {
                sitOnSofa(id, tid);
            }
            pthread_mutex_unlock(&checkSofaMutex);
        }
        sleep(4);
        sofaCounter--;
    }
    //if patient cannot enter the waiting room
    else
    {
        pthread_mutex_unlock(&waitingRoomMutex);
    }
}

//medical professional thread function
void* medProfThread(void *arg)
{
    //gets medical professional data object
    medProf *data = (medProf *)arg;
    int id = data->id;
    int tid = data->tid;
}

int main()
{
    //sets thread number
    numOfThreads = numOfPatients + numOfMedProfs;

    //initiliazes our arrays
    patientStatusArr = arrInit(numOfPatients, patientStatusArr);
    medProfStatusArr = arrInit(numOfMedProfs, medProfStatusArr);
    waitingRoomTimeArr = arrInitDouble(numOfPatients, waitingRoomTimeArr);
    sofaWaitingTimeArr = arrInitDouble(numOfPatients, sofaWaitingTimeArr);
    threadActivateArr = arrInit(numOfPatients, threadActivateArr);

    pthread_mutex_init(&waitingRoomMutex, NULL);
    pthread_mutex_init(&waitingRoomTimeMutex, NULL);
    pthread_mutex_init(&checkSofaMutex, NULL);

    //initializes threads and patient/medical professional arrays
    pthread_t threads[numOfThreads];
    patient patientData[numOfPatients];
    medProf medProfData[numOfMedProfs];

    for(int i = 0; i < numOfThreads; i++)
    {

        //creates the medical professional threads
        if(i < numOfMedProfs)
        {
            medProfData[i].id = i;
            medProfData[i].tid = i;
            medProfData[i].waitForPatientTime = 0;

            pthread_create(&threads[i], NULL, medProfThread, &medProfData[i]);           
        }
        //creates the patient threads
        else
        {
            patientData[i - numOfMedProfs].id = i - numOfMedProfs;
            patientData[i - numOfMedProfs].tid = i;
            patientData[i - numOfMedProfs].waitingRoomTime = 0;
            patientData[i - numOfMedProfs].sofaWaitTime = 0;

            pthread_create(&threads[i], NULL, patientThread, &patientData[i - numOfMedProfs]);

        }
    }

    //activates the patient threads every second
    for(int i = 0; i < numOfPatients; i++)
    {
        //sleep(1);
        
        int millisecond = 10;
        usleep(millisecond * 1000);
        threadActivateArr[i] = 1;
    }

    //joins the threads so program waits on their activation
    int res;
    for (int i = 0; i < numOfThreads; ++i)
    {
        res = pthread_join(threads[i], NULL);
        //printf("%d\n", res);
    }
    printf("ALL HAVE BEEN JOINED\n");

    printf("average waiting room time: %f\n", getAvgWaitingRoomTime());

}