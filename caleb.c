//Group A
//Name: Caleb Newport
//email:
//Date: 4/15/2022

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

//makePayment to doctor
void makePayment(int id, int tid, int mid)
{
    blue();
    printf("Patient %3i (Thread ID: %5i): Making payment to Medical Professional %3i\n", id, tid, mid);

    pthread_mutex_lock(&accessPID);
    patientStatusArr[id] = 5;
    pthread_mutex_unlock(&accessPID);
    resetColor();
}

//accepts payment 
void acceptPayment(int id, int tid, int pid)
{
    sem_wait(&transactionSemaphore);
    printf("Medical Professional %3i (Thread ID: %5i): Accepting Payment from Patient %3i\n", id, tid, pid);
    sem_post(&transactionSemaphore);

    pthread_mutex_lock(&accessPID);
    patientStatusArr[pid] = 6;
    pthread_mutex_unlock(&accessPID);
}
//patient leaves clinic
void leaveClinic(int id, int tid)
{
    yellow();
    printf("Patient %3i (Thread ID: %5i): Leaving the clinic after receiving checkup\n", id, tid);
    resetColor();

    successfullCheckupCounter++;//adds to successfull checkups
    
}