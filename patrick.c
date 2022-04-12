#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

void GetMedicalCheckup(int *id) {
    sem_wait(&Professional_Ready); // wait for medical professional to be ready

    sem_post(&Professional_Ready); // oh boi, he occupied with patient
    printf("Medical Professional %d (Thread ID: %ld): Starting Patient Checkup.\n" , *id, pthread_self());
    sem_post(&PerformCheckup);
    printf("Medical Professional %d (Thread ID: %ld): Patient Checkup Complete.\n" , *id, pthread_self());
    sem_post(&Professional_Ready); // he ready
}

void PerformMedicalCheckup(int *id) {
    sem_wait(&PerformCheckup);
    printf("Patient %d (Thread ID: %ld): Performing Medical Checkup.\n" , *id, pthread_self());
    sleep(waitTime);
    sem_post(&PerformCheckup);
}



