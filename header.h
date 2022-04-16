//Group A
//Name: Jarrett Mitchener
//email:jarrett.mitchener@okstate.edu
//Date: 4/15/2022

#ifndef HEADER_H
#define HEADER_H

//array initializers
int* arrInit(int n, int *arr, int j);
double* arrInitDouble(int n, double *arr, double j);

//print colors and reset color
void red();
void yellow();
void purple();
void green();
void blue();
void cyan();
void gray();
void resetColor();

//These are shared variables that will be used throughout 
//all of the files, not just jarretts

//parameters for program
extern int numOfPatients;
extern int numOfMedProfs;
extern int numOfThreads;
extern int numOfSofas;
extern int numOfWaitingRoomSlots;

//variables gained from command line arg
extern int numberOfMedicalProfessionals, numberOfPatients, waitingRoomCapacity, numberOfSofa;
extern int maximumArrivalTime, perPatientCheckupTime;

//counters
extern int sofaCounter;
extern int waitingRoomCounter;
extern int successfullCheckupCounter;
extern int unsuccessfullCheckupCounter;

//arrays used for logic
extern int *patientStatusArr;
extern int *medProfStatusArr;
extern int *endFlagArr;

//arrays used to keep time values
extern double *waitingRoomTimeArr;
extern double *sofaWaitingTimeArr;
extern double *totalWaitTimeArr;
extern double *totalTimeWaitingForPatients;

//mutexes used
extern pthread_mutex_t waitingRoomMutex;
extern pthread_mutex_t waitingRoomTimeMutex;
extern pthread_mutex_t checkSofaMutex;
//extern pthread_mutex_t canGetCheckupMutex;
extern pthread_mutex_t checkForPatientsMutex;
extern pthread_mutex_t checkupMutex;
extern pthread_mutex_t changePID;
extern pthread_mutex_t accessPID;

//semaphores that are used
extern sem_t transactionSemaphore;
extern sem_t canGetCheckupSemaphore;

//struct for patient data type
typedef struct patient
{
    int tid;
    int id;

} patient;

//struct for medical professional data type
typedef struct medProf
{
    int tid;
    int id;
} medProf;

#endif