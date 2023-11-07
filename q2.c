/**************************************************************/
/* bubblesort.c -- bubble sort                                 */
/* This program sorts a list of n elements in ascending  order*/
/* 1. make a loop & compare the iterated element w. next array element & swap to match criteria */
/* 2. loop the above till all elements have been iterated through */
/*************************************************************/
#include <stdio.h>  //'stdio.h' allow for standard input/output functions
#include <stdlib.h> //'stdlib.h' include functions like xand() for random number generation
#include <time.h> //'time.h' provides time-related functions and types for measuring execution time.
#include <math.h> //'math.h' provides functions like pow() 
#define MAX_SIZE 202 //A preprocessor directive that defines a symbolic name 'MAX_SIZE'. This defined value will be used as the maximum size of the array to be sorted.
void bubblesort(int x[], int n); // This is a function prototype declaration. It tells the compiler that there's a function names 'bubblesort' defined somewhere in the code

int main() //This is the entry point of the program.
{
    int x[MAX_SIZE];            /*Integer array that will store the number to be sorted*/
    int num_items;              /*an integer that will store the number of items in the list*/
    int i;                      /*loop counter        */
    int time_spent=0.0;         /*a variable to hold the time taken for the sorting algorithm*/

    /*read in array to be sorted*/
    num_items=MAX_SIZE;
    for (i=0;i<num_items;++i)
    {
        x[i]=rand();
        printf("\n%d",x[i]);    //This code block initializes the array with random values and print them. It also let the user know the size
    }                           //of the array and the randomly generated values.
    
    
    /*activate bubble sort and kick start the timing*/
    clock_t begin =clock();     /*start timing*/  //This block of code measures the time taken to sort the array using the bubble sort algorithm. The 'clock()' function
    bubblesort(x,num_items);                      // returns the processor time consumed by the program. The difference between 'end' and 'begin' gives the number of clock ticks 
    clock_t end = clock();      /*end timing*/    // taken by the 'bubblesort' function, which is then divided by 'CLOCK_PER_SEC' to get the time in seconds.
    time_spent += ((double)(end-begin)/CLOCKS_PER_SEC) * pow(10, 9);
    
    
    /*print sorted list*/
    printf("\n The sorted list is ");
    for (i=0;i<=num_items-1;++i)
    {
        printf("\n%d",x[i]);         
    }                               
    printf("\n\n Time taken to sort the array using bubble sort algorithm is %d nanoseconds\n", time_spent); //This code block prints the sorted array and the time taken for sorting.
    return 0;                                                        // Finally the main function returns 0, indicating successful termination
}
/********************************************************/
/*  function : bubble sort                              */
/*  sort an array in ascending order                    */
/*  Input :                                             */
/*    x[]-array to be sorted                            */
/*    n  - number of element in the array               */
/*   Return:                                           */
/*    x[]-sorted array                                  */
/*******************************************************/
void bubblesort(int x[], int n)     //This is the definition of the 'bubblesort' function, which takes an array 'x[]' and its size 'n' as arguement
{
    int pass;       /*pass number to keep track of how many passes have been made*/
    int i=0;        /*loop counter*/ 
    int temp;    /*temporary storage during swapping of elements*/ 

    for (pass=0;pass<n-1;++pass)
    {
        for (i=0;i<n-1;++i)     //This is the bubble sort algorithm.
        {                       //The outer loop represents the number of passes. For each pass, the largest element bubbles up to its correct position
            if (x[i]>x[i+1])    //The inner loop iterates through the list, comparing each element with the next and swapping them if they are in the wrong order
            {
                temp=x[i];      //The swapping is done using the 'temp' variable'.
                x[i]=x[i+1];
                x[i+1]=temp;
            }
        }
    }
} 

