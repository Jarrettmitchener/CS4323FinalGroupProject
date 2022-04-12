#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

void acceptPayment(int *id)
{
    sem_wait(&REGISTER_OPEN); //checks to see if the register is open or not.
    sem_post(&SOMEONE_AT_REGISTER); //lets the patient know someone is at the register.
    printf("Medical Professional %d (Thread ID: %ld): Accepting Payment.\n" , *id, pthread_self());
    sem_wait(&PAYMENT_MADE); //waits for patient to make payment.
    sem_post(&REGISTER_OPEN); //opens the register again.
}

void makePayment(int *id)
{
    sem_wait(&SOMEONE_AT_REGISTER); // semaphore to make sure there is someone there to accept the payment.
    printf("Patient %d (Thread ID: %ld): Making Payment.\n" , *id, pthread_self());
    sem_post(&PAYMENT_MADE);// this lets the person accepting the payment that the payment has been made.
}

void leaveClinic(int *id)
{
    printf("Patient %d (Thread ID: %ld): Leaving Clinic after receiving checkup.\n" , *id, pthread_self());
}


