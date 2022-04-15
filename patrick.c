#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

void getMedicalCheckup(int id, int tid)
{
    cyan();
    printf("Patient %3i (Thread ID: %5i): Getting checkup\n", id, tid);
    resetColor();
    pthread_mutex_lock(&accessPID);
    patientStatusArr[id] = 2;
    pthread_mutex_unlock(&accessPID);
    sofaCounter--;//frees up sofa space
    waitingRoomCounter--;//frees up waiting room space
    
}

void PerformMedicalCheckup(int id, int tid, int pid)
{
    purple();
    printf("Medical Professional %3i (Thread ID: %5i): Checking Patient %3i\n", id, tid, pid);
    resetColor();
    sleep(7);
    pthread_mutex_lock(&accessPID);
    patientStatusArr[pid] = 4;
    pthread_mutex_unlock(&accessPID);
}
//returns id of open MedProf
int getOpenMedProf()
{
    
    for(int i = 0; i < numOfMedProfs; i++)
    {
        if(medProfStatusArr[i] == -1)
        {
            return i;
        }
    }
}

//checks to see if there is a patient available
//if there is a patient, return id of that patient (pid)
int checkForPatients()
{
    for(int i = 0; i < numOfPatients; i++)
    {
        if(patientStatusArr[i] == 2)
        {
            pthread_mutex_lock(&accessPID);
            patientStatusArr[i] = 3;
            pthread_mutex_unlock(&accessPID);
            return i;
            
        }
    }

    return -1;
}

