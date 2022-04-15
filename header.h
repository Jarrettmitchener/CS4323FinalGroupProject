#ifndef HEADER_H
#define HEADER_H

int getNum();

int* arrInit(int n, int *arr, int j);
double* arrInitDouble(int n, double *arr, double j);

void red();
void yellow();
void purple();
void green();
void blue();
void cyan();
void gray();
void resetColor();

extern int numOfPatients;
extern int numOfMedProfs;
extern int numOfThreads;
extern int numOfSofas;
extern int numOfWaitingRoomSlots;

extern int sofaCounter;
extern int waitingRoomCounter;
extern int successfullCheckupCounter;
extern int unsuccessfullCheckupCounter;

extern int *patientStatusArr;
extern int *medProfStatusArr;
extern int *endFlagArr;

extern double *waitingRoomTimeArr;
extern double *sofaWaitingTimeArr;
extern double *totalWaitTimeArr;
extern double *totalTimeWaitingForPatients;


extern pthread_mutex_t waitingRoomMutex;
extern pthread_mutex_t waitingRoomTimeMutex;
extern pthread_mutex_t checkSofaMutex;
extern pthread_mutex_t canGetCheckupMutex;
extern pthread_mutex_t checkForPatientsMutex;
extern pthread_mutex_t checkupMutex;
extern pthread_mutex_t transactionMutex;
extern pthread_mutex_t changePID;
extern pthread_mutex_t accessPID;

extern sem_t leaveSemaphore;

typedef struct patient
{
    int tid;
    int id;
    double waitingRoomTime;
    double sofaWaitTime;
} patient;

typedef struct medProf
{
    int tid;
    int id;
    double waitForPatientTime;
} medProf;

#endif