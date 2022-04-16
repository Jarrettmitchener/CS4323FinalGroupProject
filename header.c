//Group A
//Name: Jarrett Mitchener
//email:jarrett.mitchener@okstate.edu
//Date: 4/15/2022


#include <stdlib.h>
#include <stdio.h>

//initializes an int array with given length and given value
int* arrInit(int n, int *arr, int j)
{
    arr = (int*)malloc(10* n);

    for(int i = 0; i < n; i++)
    {
        arr[i] = j;
    }
    return arr;
}

//initializes an double array with given length and given value
double* arrInitDouble(int n, double *arr, double j)
{
    arr = (double*)malloc(10* n);

    for(int i = 0; i < n; i++)
    {
        arr[i] = j;
    }
    return arr;
}

//these utility functions just change the color of the printed out
//just for ease of use
void red() 
{
    printf("\033[1;31m");
}
void yellow() 
{
    printf("\033[1;33m");
}
void purple()
{
    printf("\033[1;35m");
}
void green()
{
    printf("\033[1;32m");
}
void blue()
{
    printf("\033[1;34m");
}
void cyan()
{
    printf("\033[1;36m");
}

void gray()
{
    printf("\033[1;37m");
}
//resets the console to default color
void resetColor() 
{
    printf("\033[0m");
}

