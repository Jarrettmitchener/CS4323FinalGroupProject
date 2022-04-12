#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t REGISTER_OPEN;
sem_t SOMEONE_AT_REGISTER;
sem_t PAYMENT_MADE;
sem_t ACCEPTED;

void acceptPayment(int *id)
{
    sem_wait(&REGISTER_OPEN); //checks to see if the register is open or not.
    sem_post(&SOMEONE_AT_REGISTER); //lets the patient know someone is at the register.
    sem_wait(&PAYMENT_MADE); //waits for patient to make payment.
    printf("Medical Professional %d (Thread ID: %ld): Accepting Payment.\n" , *id, pthread_self());
    sem_post(&ACCEPTED);
    sem_post(&REGISTER_OPEN);//opens the register again.
}

void makePayment(int *id)
{
    sem_wait(&SOMEONE_AT_REGISTER); // semaphore to make sure there is someone there to accept the payment.
    printf("Patient %d (Thread ID: %ld): Making Payment.\n" , *id, pthread_self());
    sem_post(&PAYMENT_MADE);// this lets the person accepting the payment that the payment has been made.
    sem_wait(&ACCEPTED);
}

void leaveClinic(int *id)
{
    printf("Patient %d (Thread ID: %ld): Leaving Clinic after receiving checkup.\n" , *id, pthread_self());
}
/*!!!!!!!!!!!!!!!EVERYTHING BELLOW IS USED JUST FOR TESTING PURPOSES!!!!!!!!!!!!*/

void* medProfDriver(void *arg)
{
    int id = 1;
    printf("Medical Professional %d created successfully...\n", id);
    acceptPayment(&id);

}

void* patientDriver(void *arg)
{
    int id = 1;
    printf("Patient %d created successfully...\n", id);
    makePayment(&id);
    leaveClinic(&id);
}


int main()
{
    sem_init(&REGISTER_OPEN,0,1);
    sem_init(&SOMEONE_AT_REGISTER,0,0);
    sem_init(&PAYMENT_MADE,0,0);
    sem_init(&ACCEPTED,0,0);
    pthread_t medProf;
    pthread_t patient;
    pthread_create(&patient,NULL, patientDriver,NULL);
    pthread_create(&medProf,NULL,medProfDriver,NULL);
    pthread_join(patient,NULL);
    pthread_join(medProf,NULL);
}


