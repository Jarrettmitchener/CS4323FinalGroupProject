#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

int amountOfSofas;
int sofaCounter = 0;
int patientsSittingOnSofa = 0;

//set method for amount of sofas in the waiting room
void setAmountOfSofas(int n)
{
    amountOfSofas = n;
}

void addToSofas()
{
    patientsSittingOnSofa++;
}

void removeFromSofas()
{
    patientsSittingOnSofa--;
}

int getNumOfPatientsOnSofas()
{
    return patientsSittingOnSofa;
}
//sitOnSofa Method
void sitOnSofa(int id, int tid)
{
    printf("Patient %3i (Thread ID %5i):Sitting on a sofa in the waiting room\n", id, tid);
}

void waitForPatient()
{
    
}