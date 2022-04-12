#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

pthread_mutex_t waitingRoomMutex;

sem_t SOMEONE_ON_COUCH;
sem_t COUCH_OPEN;
sem_t DOCTOR_OPEN;
sem_t DOCTOR_BUSY;

typedef struct __medProf {
    int tid;
    double waitForPatient;
} medProf;

typedef struct __patient {
    int tid;
    double waitTime;
} patient;

//proceeding thread, 
void* sitOnSofa(void *id){
    //makes sure that someone is on couch
    sem_wait(&SOMEONE_ON_COUCH);
    sleep(1);
    //releases semaphore on couch
    sem_post(&SOMEONE_ON_COUCH);
    printf("Patient %d (Thread ID: %ld): Sitting on sofa in waiting room", &id, pthread_self());
}

void* waitForPatient(void *id){
    sem_wait(&DOCTOR_OPEN);
    sleep(1);
    sem_post(&DOCTOR_OPEN);

    printf(printf("Medical Professional %d (Thread ID: %ld): Waiting for a patient ", &id, pthread_self());
}


int main(){
        double *timeArr = malloc(1000);

    //getting time of each thread currently waiting on couch 
    for(int i = 0; i < numOfThreads; i++){
        timeArr = patientData[i]->waitTime;
    }

    size_t n = sizeof(timeArr)/sizeof(timeArr[0]);
    int temp = 0;
    //descending sort to get highest wait value
    for (int i = 0; i < n; i++){
        for (j = i + 1; j < n; ++j){
            if (patientData[i] < patientData[j]){
                temp = patientData[i];
                patientData[i] = patientData[j];
                patientData[j] = temp;
            }
        }
    }

    //if statement that decides thread with highest wait time sits down once couch is open
    if(parameter == patientData[0]){

    }
}
