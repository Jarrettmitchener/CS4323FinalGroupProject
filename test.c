#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )  {

   printf("The arguments are: %s %s %s %s %s %s\n", argv[1], argv[2],argv[3],argv[4],argv[5],argv[6]);
   int numberOfMedicalProfessionals, numberOfPatients, waitingRoomCapacity, numberOfSofa, maximumArrivalTime, perPatientCheckupTime;
   numberOfMedicalProfessionals = atoi(argv[1]);
   numberOfPatients = atoi(argv[2]);
   waitingRoomCapacity = atoi(argv[3]);
   numberOfSofa = atoi(argv[4]);
   maximumArrivalTime = atoi(argv[5]);
   perPatientCheckupTime = atoi(argv[6]);

   //printf("The arguments are: %i %i %i %i %i %i\n", nu, b, c, d, e, f);
}