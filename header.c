#include <stdlib.h>
#include <stdio.h>

int getNum()
{
    return 5;
}

int* arrInit(int n, int *arr)
{
    arr = (int*)malloc(10* n);

    for(int i = 0; i < n; i++)
    {
        arr[i] = -1;
    }
    return arr;
}

double* arrInitDouble(int n, double *arr)
{
    arr = (double*)malloc(10* n);

    for(int i = 0; i < n; i++)
    {
        arr[i] = -1;
    }
    return arr;
}

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

void resetColor() 
{
    printf("\033[0m");
}
