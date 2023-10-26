#include <stdio.h>

int main()
{
    double x[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0, 120.0, 140.0, 160.0, 180.0, 200.0, 220.0, 240.0, 260.0, 280.0, 300.0};
    int dataA[] = {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1};
    int dataB[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    int numPoints = sizeof(x) / sizeof(x[0]);

    // Open a pipe to gnuplot
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

    if (gnuplotPipe == NULL)
    {
        perror("Could not open gnuplot pipe");
        return 1;
    }

    // Send commands to gnuplot to create a multiplot
    fprintf(gnuplotPipe, "set multiplot layout 2, 1\n");
    fprintf(gnuplotPipe, "set title 'DataA'\n");
    fprintf(gnuplotPipe, "set border 2\n");
    fprintf(gnuplotPipe, "set tmargin at screen 0.85\n");
    fprintf(gnuplotPipe, "set bmargin at screen 0.45\n");
    fprintf(gnuplotPipe, "set yrange [-0.5:1.5]\n");
    fprintf(gnuplotPipe, "unset xtics\n");
    fprintf(gnuplotPipe, "set ytics 0,1\n");
    fprintf(gnuplotPipe, "plot '-' with steps\n");

    for (int i = 0; i < numPoints; i++)
    {
        fprintf(gnuplotPipe, "%lf %d\n", x[i], dataA[i]);
    }

    // End the first plot
    fprintf(gnuplotPipe, "e\n");

    fprintf(gnuplotPipe, "set border 3\n");
    fprintf(gnuplotPipe, "set bmargin at screen 0.1\n");
    fprintf(gnuplotPipe, "set tmargin at screen 0.45\n");
    fprintf(gnuplotPipe, "set yrange [-0.5:1.5]\n");
    // fprintf(gnuplotPipe, "set format x '%lf'\n");
    fprintf(gnuplotPipe, "set xtics auto\n");
    fprintf(gnuplotPipe, "set tics nomirror\n");
    fprintf(gnuplotPipe, "set xtics (");

    for (int i = 0; i < numPoints; i++)
    {
        fprintf(gnuplotPipe, "'%.0f' %.0f", x[i], x[i]);
        if (i < numPoints - 1)
        {
            fprintf(gnuplotPipe, ",");
        }
    }

    fprintf(gnuplotPipe, ")\n");
    fprintf(gnuplotPipe, "plot '-' with steps\n");

    for (int i = 0; i < numPoints; i++)
    {
        fprintf(gnuplotPipe, "%lf %d\n", x[i], dataB[i]);
    }

    // End the second plot and multiplot
    fprintf(gnuplotPipe, "e\n");
    fprintf(gnuplotPipe, "unset multiplot\n");

    // Close the pipe
    fclose(gnuplotPipe);

    return 0;
}
