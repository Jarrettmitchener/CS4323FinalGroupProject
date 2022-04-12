#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>


sem_t Professional_Ready;
sem_t PerformCheckup;

void GetMedicalCheckup(int *id) {
    sem_wait(&Professional_Ready); // wait for medical professional to be ready

    sem_post(&Professional_Ready); // oh boi, he occupied with patient
    printf("Medical Professional %d (Thread ID: %ld): Getting Patient Checkup.\n" , *id, pthread_self());
    sem_post(&PerformCheckup);
    printf("Medical Professional %d (Thread ID: %ld): Patient Checkup Complete.\n" , *id, pthread_self());
    sem_post(&Professional_Ready); // he ready
}

void PerformMedicalCheckup(int *id) {
    sem_wait(&PerformCheckup);

    printf("Patient %d (Thread ID: %ld): Performing Medical Checkup.\n" , *id, pthread_self());
    sleep(5);
    sem_post(&PerformCheckup);
}

void* medProfDriver(void *arg) {
    int id = 1;
    printf("Medical Professional %d created successfully...\n", id);
    GetMedicalCheckup(&id);
}

void* patientDriver(void *arg) {
    int id = 1;
    printf("Patient %d created successfully...\n", id);
    PerformMedicalCheckup(&id);
}

int main() {
    sem_init(&Professional_Ready,0,1);
    sem_init(&PerformCheckup,0,1);
    pthread_t medProf;
    pthread_t patient;
    pthread_create(&patient,NULL, patientDriver,NULL);
    pthread_create(&medProf,NULL, medProfDriver,NULL);
    pthread_join(medProf,NULL);
}



