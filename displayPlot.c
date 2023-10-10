/*
displayPlot.c

This program reads in the generated LED waveform data from the Raspberry Pi
and displays the data in graphs using GNUplot
*/

// header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINITIONS
#define LINE_LENGTH 1024 // Assume maximum line length of 1024

// function prototypes

// main function
int main()
{
    // initialise variables
    char data[LINE_LENGTH]; // size of 1024 characters for each line of data
    int row = 0;            // index of rows
    int column = 0;         // index of columns

    // open the csv file
    FILE *csvFile = fopen("data.csv", "r");

    // check if file exists
    if (csvFile == NULL)
    {
        printf("Error opening file! File does not exist!\n");
        return 1;
    }

    while (fgets(data, sizeof(data), csvFile))
    {
        char *token = strtok(data, ",");

        while (token != NULL)
        {
            printf("%s\n", token);

            token = strtok(NULL, ",");
        }
    }
}
