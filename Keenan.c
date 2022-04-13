#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

//just in case
pthread_mutex_t waitingRoomMutex;

//semaphore declaration
sem_t SOMEONE_ON_COUCH;
sem_t COUCH_OPEN;
sem_t DOCTOR_OPEN;
sem_t DOCTOR_BUSY;

//struct definitions
typedef struct __medProf {
    int tid;
    double waitForPatient;
} medProf;

typedef struct __patient {
    int tid;
    double waitTime;
} patient;

//function for patient that is waiting on couch/who is next with semaphore
void sitOnSofa(int *id){
    //makes sure that someone is on couch
    sem_wait(&SOMEONE_ON_COUCH);
    sleep(1);
    //releases semaphore on couch
    sem_post(&SOMEONE_ON_COUCH);
    printf("Patient %d (Thread ID: %ld): Sitting on sofa in waiting room\n", &id, pthread_self());
}

//function for medProf that is waiting on patient w/ semaphore
void waitForPatient(int id){
    sem_wait(&DOCTOR_OPEN);
    sleep(1);
    sem_post(&DOCTOR_OPEN);
    printf("Medical Professional %d (Thread ID: %ld): Waiting for a patient\n", &id, pthread_self());
}



//-----FOR TESTING------------
void* sitOnSofaDrive(void *id){

    sitOnSofa(id);
}

void* waitForPatientDrive(void *id){

    waitForPatient(id);
}

int main(){

    //creating patient struct in main
    patient* patientData = malloc(10000);

    //initializing semaphore with testing parameters
    sem_init(&SOMEONE_ON_COUCH, 0, 1);
    sem_init(&DOCTOR_OPEN, 0, 1);

    //DEV PURPOSE ONLY
    int numOfThreads = 10;


    //array that holds time information
    double *timeArr = malloc(1000);

    //thread creation
    pthread_t threadTest1;
    pthread_t threadTest2;

    int countArr[30];
    int demoID;

    //running threads with incrementing i
    for(int i = 0; i < numOfThreads, ++i;) {


        int *count = malloc(sizeof(int));
        *count = i;

        pthread_create(&threadTest1, NULL, sitOnSofaDrive, count);
        pthread_create(&threadTest2, NULL, waitForPatientDrive, count);
    }
    //getting time of each thread currently waiting on couch
//    for(int i = 0; i < numOfThreads; i++){
//        timeArr[i] = patientData->waitTime;
//    }

    //finding size of array
    size_t n = sizeof(timeArr) / sizeof(double);

    int temp = 0;
    //descending sort to get highest wait value
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; ++j){
            if (timeArr[i] < timeArr[j]){
                temp = timeArr[i];
                timeArr[i] = timeArr[j];
                timeArr[j] = temp;
            }
        }
    }

    // // if statement that decides thread with highest wait time sits down once couch is open
    // // if(parameter == patientData[0]){

    // }
}
