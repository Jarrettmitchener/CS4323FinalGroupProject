//driver file for CS4323 Final Group Project
//Group A
//Date: 4/11/2022
//single file that combines everyone's code for ea  se of use for the TA
//sections are marked by initials
//function are marked at the top with the initial
//if there are no initials it was a decided structure on the group
//jm - Jarrett Mitchener
//kh - Keenan Holsapple
//cn - Caleb Newport
//pl - Patrick Laverty




#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

//mutexes and semaphore variables declared here 
pthread_mutex_t waitingRoomMutex;//jm
sem_t sofaSemaphore;//jm
sem_t SOMEONE_AT_REGISTER;//cn
sem_t PAYMENT_MADE;//cn
sem_t REGISTER_OPEN;//cn



//counters used for functions
int sofaCounter = 0;//jm
int waitingroomCounter = 0;//jm

//variables that are used in testing
//will be removed later when program arguments are added;
int numOfSofas = 3;//jm
int spaceOfWaitingRoom = 5;//jm
int numOfPatients = 8;
int numOfMedicalProf = 2;
int numOfThreads;//jm

//boolean array that will be used to determine when threads fire
int flag[15];//jm

typedef struct medProf {
    int tid;
    int id;
    double waitForPatient;
} medProf;

typedef struct patient {
    int tid;
    int id;
    double waitTime;
} patient;

//function to add people to waiting room
//jm
int addWaitingRoom()
{
    if(waitingroomCounter == spaceOfWaitingRoom)
    {
        return 0;
    }
    else 
    {
        waitingroomCounter++;
        return 1;
    }
}

//initializes the flag of when the threads fire
//jm
void flag_init()
{
    for(int i = 0; i < numOfThreads; i++)
    {
        flag[i] = 0;
    }
}

//function for medical professional threads
void* medProfFunc(void *arg)
{
    medProf *data = (medProf *)arg;//jm
    int id = data->id;//jm
    int tid = data->tid;//jm
    //printf("med ID %i has been made\n", id);
}

//function for patient threads
void* patientFunc(void *arg)
{
    patient *data = (patient *)arg;//jm
    int id = data->id;//jm
    int tid = data->tid;//jm

    //printf("patient ID %i has been made\n", id);
    

    //thread waits so it can only start when told to by main
    while(flag[id] == 0);//jm
    //printf("Thread ID %i has started\n", id);


    //critical section for waiting room entering
    pthread_mutex_lock(&waitingRoomMutex);//jm
    printf("Patient %3i (Thread ID %5i):Arriving at clinic\n", id, tid);//jm
    int leave = addWaitingRoom();//jm
    if(leave == 0)
    {
        printf("Patient %3i (Thread ID %5i):Leaving clinic without checkup\n", id, tid);//jm
        //pthread_exit(NULL);//temp exit method because thread shouldn't do anything now
        pthread_mutex_unlock(&waitingRoomMutex);//jm
    }
    else
    {
        printf("Patient %3i (Thread ID %5i):Going into the clinic\n", id, tid);//jm

        sleep(2);//jm
        pthread_mutex_unlock(&waitingRoomMutex);//jm
        //end of entering waiting room critical section

        //start of sofa semaphore critical section
        sem_wait(&sofaSemaphore);//jm

        printf("Patient %3i (Thread ID %5i):Sitting on a sofa in the waiting room\n", id, tid);//jm
        sleep(2);//jm
        sem_post(&sofaSemaphore);//jm
        //end of sofa section
    }
    
}

//cn
void makePayment(int *id)
{
    sem_wait(&SOMEONE_AT_REGISTER); // semaphore to make sure there is someone there to accept the payment.
    printf("Patient %d (Thread ID: %ld): Making Payment.\n" , *id, pthread_self());
    sem_post(&PAYMENT_MADE);// this lets the person accepting the payment that the payment has been made.
}

//cn
void acceptPayment(int *id)
{
    sem_wait(&REGISTER_OPEN); //checks to see if the register is open or not.
    sem_post(&SOMEONE_AT_REGISTER); //lets the patient know someone is at the register.
    printf("Medical Professional %d (Thread ID: %ld): Accepting Payment.\n" , *id, pthread_self());
    sem_wait(&PAYMENT_MADE); //waits for patient to make payment.
    sem_post(&REGISTER_OPEN); //opens the register again.
}



int main()
{
    numOfThreads = numOfPatients + numOfMedicalProf;
    sem_init(&sofaSemaphore, 0, numOfSofas);//jm
    sem_init(&SOMEONE_AT_REGISTER, 0, 1); //cn
    sem_init(&PAYMENT_MADE, 0, 1);//cn
    sem_init(&REGISTER_OPEN, 0, 1);//cn

    pthread_mutex_init(&waitingRoomMutex, NULL);//jm

    pthread_t threads[numOfThreads];//jm
    patient patientData[numOfPatients];//jm
    medProf medProfData[numOfMedicalProf];//jm

    int rc;//jm
    for(int i = 0; i < numOfThreads; i++)//jm
    {
        //assigns patient data

        if(i < numOfMedicalProf)//jm
        {
            medProfData[i].id = i;//jm
            medProfData[i].tid = i;//jm
            if ((rc = pthread_create(&threads[i], NULL, medProfFunc, &medProfData[i])))//jm
            {
                //this will only run if the thread cannot be created 
                fprintf(stderr, "error creating medical professional thread\n");//jm
                return EXIT_FAILURE;//jm
            }
        }
        else
        {
            patientData[i - numOfMedicalProf].id = i - numOfMedicalProf;//jm
            patientData[i - numOfMedicalProf].tid = i;//jm
            if ((rc = pthread_create(&threads[i], NULL, patientFunc, &patientData[i - numOfMedicalProf])))//jm
            {
                //this will only run if the thread cannot be created 
                fprintf(stderr, "error creating patient thread\n");//jm
                return EXIT_FAILURE;//jm
            }
        }
    }

    //activates the patient threads every second
    for(int i = 0; i < numOfThreads; i++)//jm
    {
        sleep(1);//jm
        flag[i] = 1;//jm
    }

    //joins the threads so program waits on their activation
    int res;//jm
    for (int i = 0; i < numOfThreads; ++i)//jm
    {
        res = pthread_join(threads[i], NULL);//jm
        //printf("%d\n", res);
    }
    printf("ALL HAVE BEEN JOINED\n");//jm
}