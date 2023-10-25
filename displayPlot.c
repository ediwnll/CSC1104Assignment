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
#define LINE_LENGTH 256 // Assume maximum line length of 1024
#define MAX_ROWS 100    // Maximum number of rows
#define MAX_COLUMNS 2   // Number of columns in CSV file

// function prototypes
void plot_gnu_step();
void testplot();

void testplot(){
    // Gnuplot commands
    const char* gnuplotCommands[] = {
        "set multiplot layout 2, 1",    // Create a 2 plot in 1 window gnuplot
        "set tmargin at screen 0.85",   //Adjust top margin to space the plot out
        "set bmargin at screen 0.45",   //adjust bottom margin to space the plot
        "set xrange [0:300]",           //set the x-axis of range, to be used with dataset
        "set tics scale 0.75",          //set tics scaling for labels
        "set xtics 20",                 //set x-axis to be at intervals of 20
        "set grid xtics",               //display grid lines for x axis tics
        "set format x ''",              //remove x axis label format
        "set ytics 0,1",                //set y-axis tics at 0 and 1
        "set grid ytics",               //display gridline for y axis
        "set format y '%.1f'",          //format y axis label
        "s1(x) = (int(x) % 20 < 10) ? 1 : 0",   //define step1 as a step function
        "s2(x) = (int(x) % 40 < 20) ? 1 : 0",   //define step2 as a step function
        "set yrange [-0.5:1.5]",        //set y axis range to be -0.5 to 1.5
        "set samples 1000",             //set sample for smooth curves
        "plot s1(x) with steps ls 1 title 'Graph 1'",   //plot s1 with the title
        "set bmargin at screen 0.1",    //adjust bottom margin of bottom graph
        "set tmargin at screen 0.45",   //adjust top margin of bottom graph
        "set yrange [-0.5:1.5]",        //set y axis range to be -0.5 to 1.5
        "set format x '%.0f'",          //set x axis label format
        "plot s2(x) with steps ls 1 title 'Graph 2'",   //plot s2 with the title
        "unset multiplot",  // Exit multiplot mode
    };

    // Open a pipe to Gnuplot
    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    //if no gnuplot, exit
    if (gnuplotPipe == NULL) {
        perror("Could not open Gnuplot.");
        exit(EXIT_FAILURE);
    }

    // Execute Gnuplot commands
    for (int i = 0; i < sizeof(gnuplotCommands) / sizeof(gnuplotCommands[0]); i++) {
        fprintf(gnuplotPipe, "%s\n", gnuplotCommands[i]);
    }

    // Close the Gnuplot pipe
    pclose(gnuplotPipe);
}

void plot_gnu_step()
{
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL)
    {
        perror("Unable to open gnuplot");
        exit(EXIT_FAILURE);
    }
    double x[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0, 120.0, 140.0, 160.0, 180.0, 200.0, 220.0, 240.0, 260.0, 280.0, 300.0};
    int dataA[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    int dataB[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

    // Send Gnuplot commands to plot both data sets
    fprintf(gnuplotPipe, "set title 'Step Functions A and B'\n");
    fprintf(gnuplotPipe, "set xlabel 'Time (ms)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Value'\n");
    fprintf(gnuplotPipe, "plot '-' with steps title 'Data A', '-' with steps title 'Data B'\n");
    for (int i = 0; i < 16; i++)
    {
        fprintf(gnuplotPipe, "%lf %d\n", x[i], dataA[i]);
    }
    fprintf(gnuplotPipe, "e\n");
    for (int i = 0; i < 16; i++)
    {
        fprintf(gnuplotPipe, "%lf %d\n", x[i], dataB[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    // Close the Gnuplot pipe
    pclose(gnuplotPipe);
}

// main function
int main()
{
    // initialise variables
    char values[LINE_LENGTH];                      // size of 1024 characters for each line of data
    char data[MAX_ROWS][MAX_COLUMNS][LINE_LENGTH]; // array to store the csv data
    int row = 0;                                   // Current row in CSV
    int col = 0;                                   // Current column in CSV

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
        // set column index to 0, indicating first column
        col = 0;
        //  split the data using comma as the delimiter
        char *value = strtok(values, ",");

        // store each value into the array
        while (value != NULL && col <= MAX_COLUMNS)
        {
            strcpy(data[row][col], value);
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
            printf("%s\t", data[i][j]);
        }
        printf("\n");
    }

    testplot();

    return 0;
}
