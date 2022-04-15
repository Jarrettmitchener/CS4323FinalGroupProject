#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#include "header.h"

#include "caleb.c"
#include "keenan.c"
#include "patrick.c"
#include "utility.c"

//#include "test.c"

//temp variables that will be changed later
int numOfPatients = 10;
int numOfMedProfs = 10;
int numOfThreads;
int numOfSofas = 3;
int numOfWaitingRoomSlots = 5;

int sofaCounter = 0;
int waitingRoomCounter = 0;
int successfullCheckupCounter = 0;
int unsuccessfullCheckupCounter = 0;

//arrays that will be used to determine the status of the patients/medical professionals
//-1: nothing has been done with them yet
int *patientStatusArr;
int *medProfStatusArr;
int *endFlagArr;

//keeps track of time for sofa and waiting room
double *waitingRoomTimeArr;
double *sofaWaitingTimeArr;
double *totalWaitTimeArr;
double *totalTimeWaitingForPatients;
//thread activation array for patient threads
int* threadActivateArr;

//mutex that will be used for adding/removing waiting room 
pthread_mutex_t waitingRoomMutex;
pthread_mutex_t waitingRoomTimeMutex;
pthread_mutex_t checkSofaMutex;
pthread_mutex_t canGetCheckupMutex;
pthread_mutex_t checkForPatientsMutex;
pthread_mutex_t checkupMutex;
pthread_mutex_t transactionMutex;
pthread_mutex_t changePID;
pthread_mutex_t accessPID;

sem_t leaveSemaphore;


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
        pthread_mutex_lock(&accessPID);
        patientStatusArr[id] = 0;//updates patient status
        pthread_mutex_unlock(&accessPID);
        pthread_mutex_unlock(&waitingRoomMutex);

        double time = getTime();//used to get final time waited for sofa
        
        int canSitOnSofa = 0;
        //while loop so that it waits until user is the one with the most time in waiting room to sit on the sofa
        while(canSitOnSofa == 0)
        {
            pthread_mutex_lock(&checkSofaMutex);

            //updates the patient wait time
            double tempTime = getTime() - time;
            waitingRoomTimeArr[id] = tempTime;
            
            //sees if patient can move to the sofa
            canSitOnSofa = canMoveToSofa(id);
            if(canSitOnSofa == 1)
            {
                sitOnSofa(id, tid);
            }
            pthread_mutex_unlock(&checkSofaMutex);
        }

        time = getTime();//resetting time so that it can be used for the amount of time waited on sofa

        int canGetMedicalCheckup = 0;
        //while loop that continues until person can get a medical checkup
        while(canGetMedicalCheckup == 0)
        {
            pthread_mutex_lock(&canGetCheckupMutex);
            //printf("%i BRUH\n", id);
            //updates the patient sofa wait time
            double tempTime = getTime() - time;
            sofaWaitingTimeArr[id] = tempTime;

            canGetMedicalCheckup = canGetCheckup(id);
            //printf("%i BRUH PART TWO value: %i\n", id, canGetMedicalCheckup);

            if(canGetMedicalCheckup == 1)
            {
                //printf("PATIENT ID %i IN IF STATEMENT\n", id);
                getMedicalCheckup(id, tid);
            }

            pthread_mutex_unlock(&canGetCheckupMutex);

        }

        pthread_mutex_lock(&changePID);
        int mid = getOpenMedProf();//gets ID of our doctor
        pthread_mutex_unlock(&changePID);

        while(patientStatusArr[id] != 4);//waits until checkup is complete

        makePayment(id, tid, mid);
        
        //printf("AFTER MAKE PAYMENT PATIENT ID %i\n", id);
        while(patientStatusArr[id] != 6);//waits until payment is accepted
        //printf("AFTER PATIENT ID %i WHILE LOOP\n", id);

        leaveClinic(id, tid);

    }
    //if patient cannot enter the waiting room
    else
    {
        pthread_mutex_unlock(&waitingRoomMutex);
    }
    endFlagArr[id] = 1;
    while(endProgram() == 0);
}

//medical professional thread function
void* medProfThread(void *arg)
{
    //gets medical professional data object
    medProf *data = (medProf *)arg;
    int id = data->id;
    int tid = data->tid;

    waitForPatient(id, tid);
    int endFlag = 0;
    while(endProgram() == 0)
    {
        int checkForPatient = -1;

        double time = getTime();

        //waits to get id of patient so we can consistently lock each medical professional with an ID
        pthread_mutex_lock(&checkForPatientsMutex);
        while(checkForPatient == -1)
        {
            checkForPatient = checkForPatients();
            if(endProgram() == 1)
            {
                endFlag = 1;
                break;
            }
        }

        if(endFlag == 1)
        {
            pthread_mutex_unlock(&checkForPatientsMutex);
            break;
        }
        time = getTime() - time;//time took to get a patient
        addWaitTimeforMedProf(time, id);

        pthread_mutex_lock(&changePID);
        medProfStatusArr[id] = 0;
        pthread_mutex_unlock(&changePID);
        int pid = checkForPatient;//gets id of patient
        pthread_mutex_unlock(&checkForPatientsMutex);

        
        PerformMedicalCheckup(id, tid, pid);

        while(patientStatusArr[pid] != 5);//waits until patient has invoked makePayment

        acceptPayment(id, tid, pid);

        pthread_mutex_lock(&changePID);
        medProfStatusArr[id] = -1;//resets the status of the medical professional so they can check up on next person
        pthread_mutex_unlock(&changePID);
    }

}

int main()
{
    //sets thread number
    numOfThreads = numOfPatients + numOfMedProfs;

    //initiliazes our arrays
    patientStatusArr = arrInit(numOfPatients, patientStatusArr, -1);
    medProfStatusArr = arrInit(numOfMedProfs, medProfStatusArr, -1);
    waitingRoomTimeArr = arrInitDouble(numOfPatients, waitingRoomTimeArr, -1);
    sofaWaitingTimeArr = arrInitDouble(numOfPatients, sofaWaitingTimeArr, -1);
    threadActivateArr = arrInit(numOfPatients, threadActivateArr, -1);
    endFlagArr = arrInit(numOfPatients, endFlagArr, -1);
    totalWaitTimeArr = arrInitDouble(numOfPatients, totalWaitTimeArr, 0);
    totalTimeWaitingForPatients = arrInitDouble(numOfMedProfs, totalTimeWaitingForPatients, 0);

    pthread_mutex_init(&waitingRoomMutex, NULL);
    pthread_mutex_init(&waitingRoomTimeMutex, NULL);
    pthread_mutex_init(&checkSofaMutex, NULL);
    pthread_mutex_init(&canGetCheckupMutex, NULL);
    pthread_mutex_init(&checkForPatientsMutex, NULL);
    pthread_mutex_init(&checkupMutex, NULL);
    pthread_mutex_init(&transactionMutex, NULL);
    pthread_mutex_init(&changePID, NULL);
    pthread_mutex_init(&accessPID, NULL);

    sem_init(&leaveSemaphore, 0, 1);

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
        sleep(1);
        
        //int millisecond = 10;
        //usleep(millisecond * 1000);
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

    printf("Statistical Summary:\n----------------------------------------------------------------\n");
    printf("Number of sucessful checkups: %i\n", successfullCheckupCounter);
    printf("Average waiting time for Medical Professionals: %f seconds\n", getAvgWaitTime());
    printf("Number of Patients that left: %i\n", unsuccessfullCheckupCounter);
    printf("Average wait time for patients: %f seconds\n", getWaitTimeForPatients());
    

}