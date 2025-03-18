#include <stdio.h>
#include <stdlib.h>
#include "patience.h"
#include "shuffle.h"
#include "histogram.h"

int main()
{
    int n = 10000;
    int num_labels = 0;
    int *matches = many_plays(n);

    int *labels = get_labels(matches, &num_labels);
    double *percentages = get_percentages(matches, n, num_labels);

    FILE *fp = fopen("phistogram.txt", "w");
    if (fp == NULL)
    {
        perror("Failed to open phistogram.txt");
        exit(1);
    }

    // Save the current stdout, redirect stdout to fp,
    // call histogram (which prints to stdout), then restore stdout.
    FILE *old_stdout = stdout;
    stdout = fp;
    histogram(labels, percentages, num_labels, 30);
    stdout = old_stdout;
    fclose(fp);

    free(labels);
    free(percentages);
    free(matches);
    return 0;
}
