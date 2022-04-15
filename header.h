#ifndef HEADER_H
#define HEADER_H

int getNum();

int* arrInit(int n, int *arr);
double* arrInitDouble(int n, double *arr);

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

extern int *patientStatusArr;
extern int *medProfStatusArr;

extern double *waitingRoomTimeArr;
extern double *sofaWaitingTimeArr;

extern pthread_mutex_t waitingRoomMutex;
extern pthread_mutex_t waitingRoomTimeMutex;
extern pthread_mutex_t checkSofaMutex;

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