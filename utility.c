//Group A
//Name: Jarrett Mitchener
//email:jarrett.mitchener@okstate.edu
//Date: 4/15/2022

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "header.h"

int waitingRoomCounter;

double avgWaitingRoomTime = 0;
int counter = 0;
int medProfCounter = 0;

//function that determines if a patient can enter the waiting room
int canEnterWaitingRoom(int id, int tid)
{
    int canEnter;
    printf("Patient %3i (Thread ID: %5i): Arriving at the clinic\n", id, tid);
    //if the waiting room has a spot
    if(waitingRoomCounter < numOfWaitingRoomSlots)
    {
        canEnter = 1;
    }
    //if waiting room is full
    else
    {
        red();
        printf("Patient %3i (Thread ID: %5i): Leaving clinic without checkup\n", id, tid);
        resetColor();
        canEnter = 0;
        unsuccessfullCheckupCounter++;
    }

    return canEnter;
}
//enter waiting room function
void enterWaitingRoom(int id, int tid)
{
    waitingRoomCounter++;//adds to amount of people in the waiting room

    printf("Patient %3i (Thread ID: %5i): Entered the waiting room\n", id, tid);

}

//gets time at time of call
double getTime()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}

//function to add to Avgwaitingroom time
void addToAvgWaitTime(double time, int pid)
{
    avgWaitingRoomTime = avgWaitingRoomTime + time;
    totalWaitTimeArr[pid] = totalWaitTimeArr[pid] + avgWaitingRoomTime;//for total wait time
    counter++;
}
double getAvgWaitingRoomTime()
{
    double avg = avgWaitingRoomTime/counter;
}
//sees if someone who is standing in the waiting room can move to the sofa
//returns 1 if can move
//returns 0 if cannot
//DOES NOT ADD TO THE SOFA COUNTER
int canMoveToSofa(int id)
{
    //if sofas are full
    if(sofaCounter == numOfSofas)
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
            if(waitingRoomTimeArr[i] > max)
            {
                max = waitingRoomTimeArr[i];
                pid = i;
            }
        }
        //if maxindex is same as id, then can add to sofa
        if(pid == id)
        {
            //adds to avgWaitTime value
            addToAvgWaitTime(waitingRoomTimeArr[pid], id);
            waitingRoomTimeArr[pid] = 0;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
//checks to see if threads will end
int endProgram()
{
    int endCounter = 0;
    for(int i = 0; i < numOfPatients; i++)
    {
        if(endFlagArr[i] == 1)
        {
            endCounter++;
        }
    }
    if (endCounter == numOfPatients)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//computes average wait time of the patients
//this includes time waiting for a sofa and a time waiting on the sofa
double getAvgWaitTime()
{
    double total = 0;
    for(int i = 0; i < numOfPatients; i++)
    {
        total = total + totalWaitTimeArr[i];
    }
    total = total/numOfPatients;
    return total;
}

//adds the total wait time for the medical professionals as well as the counter
void addWaitTimeforMedProf(double time, int id)
{
    totalTimeWaitingForPatients[id] = totalTimeWaitingForPatients[id] + time;//adds wait time to arr
    medProfCounter++;
}

//computes the average waiting time medical profesionals had to wait for patients
double getAvgWaitTimeForPatients()
{
    double total = 0;
    for(int i = 0; i < numOfMedProfs; i++)
    {
        total = total + totalTimeWaitingForPatients[i];
    }
    total = total / medProfCounter;
    //if(total < 0)
    //{
    //    total = 0;
    //}
    return total;
}