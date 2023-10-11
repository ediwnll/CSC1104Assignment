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
#define MAX_ROWS 100     // Maximum number of rows
#define MAX_COLUMNS 2    // Number of columns in CSV file

// function prototypes

// main function
int main()
{
    // initialise variables
    char values[LINE_LENGTH];        // size of 1024 characters for each line of data
    int data[MAX_ROWS][MAX_COLUMNS]; // array to store the csv data
    int row = 0;                     // Current row in CSV
    int col = 0;                     // Current column in CSV

    // open the csv file
    FILE *csvFile = fopen("data.csv", "r");

    // check if file exists
    if (csvFile == NULL)
    {
        printf("Error opening file! File does not exist!\n");
        return 1;
    }

    // read CSV file line by line
    while (fgets(values, sizeof(values), csvFile))
    {
        // split the data using comma as the delimiter
        char *value = strtok(values, ",");

        // store each value into the array
        while (value)
        {
            data[row][col] = atoi(value);
            value = strtok(NULL, ",");
            col++;
        }

        // move to next row
        row++;

        // Check if we have reached the maximum number of rows
        if (row >= MAX_ROWS)
        {
            printf("Maximum number of rows reached (%d). Increase MAX_ROWS if needed.\n", MAX_ROWS);
            break;
        }
    }

    // close the CSV file
    fclose(csvFile);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            printf("%d", data[i][j]);
        }
        printf("\n");
    }

    return 0;
}
