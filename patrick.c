#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_ALLOWED 2


int waitTime = 10;
int numMedicalProfessionals = 2;


typedef struct medProf {
    int tid;
    int id;
    double waitForPatient;
} medProf;

sem_t patientReady;

void* createMedProf(void *arg)
{
    medProf *data = (medProf *)arg;
    int id = data->id;
    int tid = pthread_self();
    data->tid = tid;

    sem_wait(&patientReady);

    // performing medical checkup
    sleep(data->waitForPatient);
    printf("Medical Professional %i (Thread: %i): Checking Patient\n", id, tid);
    

    sem_post(&patientReady);
    printf("Medical Professional %i (Thread: %i): Done Checking Patient\n", id, tid);
}

int main()
{
    sem_init(&patientReady, 0, NUM_ALLOWED);

    pthread_t threads[2];
    medProf newProf[2];

    int rc, i;
    for (i = 0; i < numMedicalProfessionals; i++) {
        newProf[i].id = i;
        newProf[i].waitForPatient = waitTime; // idk if this is right
        if ((rc = pthread_create(&threads[i], NULL,  &newProf[i], NULL)))
        {
            fprintf(stderr, "error: pthread_create, createMedProf: %i\n", rc);
            return EXIT_FAILURE;
        }
    }
    
    int res;
    for (i = 0; i < 10; ++i)
    {
        res = pthread_join(threads[i], NULL);
    }
}
