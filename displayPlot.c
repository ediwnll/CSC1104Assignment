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
#define LINE_LENGTH 256 // Assume maximum line length of 256
#define MAX_ROWS 300   // Maximum number of rows
#define MAX_COLUMNS 8   // Number of columns in CSV file
#define MIDDLE_COL 4    // variable to split the data into green and red LED data arrays

// function prototypes
void gnuPlot(char greenData[MAX_ROWS][MIDDLE_COL][LINE_LENGTH], char redData[MAX_ROWS][MIDDLE_COL][LINE_LENGTH], int max_rows);

// main function
int main()
{
    // initialise variables
    char values[LINE_LENGTH];                          // size of 256 characters for each line of data
    char greenData[MAX_ROWS][MIDDLE_COL][LINE_LENGTH]; // array to store the csv data
    char redData[MAX_ROWS][MIDDLE_COL][LINE_LENGTH];   // array to store the csv data

    int row = 0; // Current row in CSV
    int col = 0; // Current column in CSV

    // open the csv file
    FILE *csvFile = fopen("displayPlot.csv", "r");

    // check if file exists
    if (csvFile == NULL)
    {
        printf("Error opening file! File does not exist!\n");
        return 1;
    }

    // Skip CSV header row
    fgets(values, sizeof(values), csvFile);

    // read CSV file line by line
    while (fgets(values, sizeof(values), csvFile))
    {
        // set column index to 0, indicating first column
        col = 0;
        //  split the data using comma as the delimiter
        char *value = strtok(values, ",");

        // store each value into the array
        while (value != NULL && col < MAX_COLUMNS)
        {
            // printf("row: %d, col: %d, value: %s\n", row, col ,value);
            if (col < MIDDLE_COL)
            {
                strcpy(greenData[row][col], value);
            }
            else
            {
                strcpy(redData[row][col - MIDDLE_COL], value);
            }
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
    gnuPlot(greenData, redData, row); // calls the function to generate the gnuplot
    return 0;
}
void gnuPlot(char greenData[MAX_ROWS][MIDDLE_COL][LINE_LENGTH], char redData[MAX_ROWS][MIDDLE_COL][LINE_LENGTH], int max_rows)
{

    double greenDataDouble[MAX_ROWS][MIDDLE_COL]; // create a double array to store green data values
    for (int i = 0; i < max_rows; i++)            // use a nested forloop to loop through the 2D array
    {
        for (int j = 0; j < MIDDLE_COL; j++)
        {
            greenDataDouble[i][j] = atof(greenData[i][j]); // convert char greenData to double greenData
        }
    }
    double redDataDouble[MAX_ROWS][MIDDLE_COL]; // create a double array to store red data values
    for (int i = 0; i < max_rows; i++)
    {
        for (int j = 0; j < MIDDLE_COL; j++)
        {
            redDataDouble[i][j] = atof(redData[i][j]); // convert char redData to double redData
        }
    }

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w"); // open pipe to gnuplot

    if (gnuplotPipe == NULL) // if the pointer points to no value
    {
        perror("Cannot open gnuplot pipe"); // print out gnuplot pipe cannot be open. Check if you installed gnuplot
    }

    // send commands for first plot
    fprintf(gnuplotPipe, "set multiplot layout 2, 1\n");                                                                               // create a multiplot
    fprintf(gnuplotPipe, "unset key\n");                                                                                               // undo the unset key
    fprintf(gnuplotPipe, "set title 'Blink Green LED at %.0f Hz, %.0f%% duty cycle'\n", greenDataDouble[0][1], greenDataDouble[0][2]); // set the title of the first graph
    fprintf(gnuplotPipe, "set border 2\n");                                                                                            // remove the top and bottom border of the graph
    fprintf(gnuplotPipe, "set tmargin at screen 0.85\n");                                                                              // adjust the top margin of the graph
    fprintf(gnuplotPipe, "set bmargin at screen 0.45\n");                                                                              // adjust the bottom margin of the graph
    fprintf(gnuplotPipe, "set yrange [-0.5:1.5]\n");                                                                                   // set the range of y to be from -0.5 to 1.5 to accomodate the actual data
    fprintf(gnuplotPipe, "set tics nomirror\n");                                                                                       // remove the tics on the top and right hand side
    fprintf(gnuplotPipe, "unset xtics\n");                                                                                             // remove the xtics from first graph as we will be using from the second graph
    fprintf(gnuplotPipe, "set key title 'waveform'\n");                                                                                // add in title for the legend
    fprintf(gnuplotPipe, "set ytics 0,1\n");                                                                                           // set the ytics to only show 0 and 1 instead of decimals
    fprintf(gnuplotPipe, "plot '-' with steps linecolor '#009933'\n");                                                                 // plot out the graph with the color being green in step line

    for (int i = 0; i < max_rows; i++) // loop through the green array
    {
        fprintf(gnuplotPipe, "%f %f\n", greenDataDouble[i][0], greenDataDouble[i][3]); // let x axis be greenDataDouble[i][0] (time iteration), and y axis to be greenDataDouble[i][3](state of green LED)
    }
    fprintf(gnuplotPipe, "e\n"); // to end data input
    // End of first plot

    // send commands for second plot
    fprintf(gnuplotPipe, "set title 'Blink Red LED at %.0f Hz, %.0f%% duty cycle'\n", redDataDouble[0][1], redDataDouble[0][2]); // set the title of the second graph
    fprintf(gnuplotPipe, "set border 3\n");                                                                                      // remove the top border of the graph
    fprintf(gnuplotPipe, "set bmargin at screen 0.1\n");                                                                         // adjust bottom margin of the graph
    fprintf(gnuplotPipe, "set tmargin at screen 0.45\n");                                                                        // adjust the top margin of the graph
    fprintf(gnuplotPipe, "set yrange [-0.5:1.5]\n");                                                                             // set the range of y to be from -0.5 to 1.5 to accomodate the actual data
    fprintf(gnuplotPipe, "set xtics auto\n");                                                                                    // auto adjust the x tics for x axis
    fprintf(gnuplotPipe, "set tics nomirror\n");                                                                                 // remove the tics on top and right hand side
    fprintf(gnuplotPipe, "plot '-' with steps linecolor 'red'\n");                                                               // plot out the graph with the color being red in step line

    for (int i = 0; i < max_rows; i++) // loop through both green and red array
    {
        fprintf(gnuplotPipe, "%lf %f\n", greenDataDouble[i][0], redDataDouble[i][3]); // let x axis be greenDataDouble[i][0] (time iteration), and y axis to be redDataDouble[i][3](state of green LED)
    }
    fprintf(gnuplotPipe, "e\n"); // to end data input
    // End of the second plot
    // close the pipe
    fclose(gnuplotPipe);
}