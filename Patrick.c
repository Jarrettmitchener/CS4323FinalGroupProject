#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>



void getMedicalCheckup()
{

}

void performMedicalCheckup()
{
    
}

int checkIfPatientOpen(int numOfPatients, int *patientArr)
{
    for(int i = 0; i < numOfPatients; i++)
    {
        if(patientArr[i] > -1)
        {
            return i;
        }
    }
    return -1;//no patient was found
}