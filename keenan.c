#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

void sitOnSofa(int id, int tid)
{
    sofaCounter++;
    green();
    printf("Patient %3i (Thread ID: %5i): Sitting on a sofa in the waiting room\n", id, tid);
    printf("sofa value: %i\n", sofaCounter);
    resetColor();
}

void waitForPatient()
{

}
