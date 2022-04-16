#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//s#include "header.h"

//extern int numOfPatients;

extern int arr[100000];

void arrInit()
{
    for(int i = 0; i < numOfPatients; i++)
    {
        arr[i] = i;
    }
}

void printNumOfPatients()
{
    printf("%i\n", numOfPatients);
}

void dec()
{
    numOfPatients--;
}