#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

int sofaTimeCounter = 0;
double avgSofaWaitingTime = 0;

//sits patient on sofa
void sitOnSofa(int id, int tid)
{
    sofaCounter++;
    green();
    printf("Patient %3i (Thread ID: %5i): Sitting on a sofa in the waiting room\n", id, tid);
    //printf("sofa value: %i\n", sofaCounter);
    pthread_mutex_lock(&accessPID);
    patientStatusArr[id] = 1;
    pthread_mutex_unlock(&accessPID);
    resetColor();
}

void waitForPatient(int id, int tid)
{
    printf("Medical Professional %3i (Thread ID: %5i): Waiting for Patient\n", id, tid);
}

//checks to see if there is an open medical professional
int isMedProfOpen()
{

    for(int i = 0; i < numOfMedProfs; i++)
    {
        //checks status of medical professionals to see if they are open
        if(medProfStatusArr[i] == -1)
        {
            return 1;
        }
    }
    return 0;
}

void addToAvgSofaWaitTime(double time, int pid)
{
    avgSofaWaitingTime = avgSofaWaitingTime + time;
    totalWaitTimeArr[pid] = totalWaitTimeArr[pid] + avgSofaWaitingTime;//for total wait time
    sofaTimeCounter++;
}

//sees if someone is waiting on the sofa
//returns 1 if can get checkup
//returns 0 if cannot
//DOES NOT REMOVE FROM SOFA COUNTER
int canGetCheckup(int id)
{
    //if medical professional is open
    if(isMedProfOpen() == 0)
    {
        return 0;
    }
    else
    {
        double max = 0;
        int pid = -1;
        //finds max index
        for(int i = 0; i < numOfPatients; i++)
        {
            if(sofaWaitingTimeArr[i] > max)
            {
                max = sofaWaitingTimeArr[i];
                pid = i;
            }
        }
        
        if(pid == id)
        {
            //adds to avgWaitTime value
            addToAvgSofaWaitTime(sofaWaitingTimeArr[pid], pid);
            sofaWaitingTimeArr[pid] = 0;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
