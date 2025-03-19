#include <stdio.h>
#include <stdlib.h>
#include "patience.h"
#include "shuffle.h"
#include "histogram.h"
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int n = 10000;              // Number of simulations
    int num_labels = 53;        // Fixed to include all possibilities: 0 to 52 cards left
    fflush(stdout);
    int *matches = many_plays(n);  // Simulate n games and get frequency of cards left

    // Allocate and populate labels array with all possible outcomes (0 to 52)
    int *labels = malloc(num_labels * sizeof(int));
    if (!labels) {
        perror("Memory allocation failed for labels");
        exit(1);
    }
    for (int i = 0; i < num_labels; i++) {
        labels[i] = i;      // Labels are 0, 1, 2, ..., 52
    }

    // Allocate and calculate percentages for all possible outcomes
    double *percentages = malloc(num_labels * sizeof(double));
    if (!percentages) {
        perror("Memory allocation failed for percentages");
        exit(1);
    }
    for (int i = 0; i < num_labels; i++) {
        percentages[i] = (matches[i] * 100.0) / n;  // Percentage for each number of cards left
    }

    // Save current stdout file descriptor
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        perror("dup failed");
        exit(1);
    }

    // Redirect stdout to the file
    FILE *fp = freopen("phistogram.txt", "w", stdout);
    if (fp == NULL) {
        perror("freopen failed");
        exit(1);
    }

    // Now histogram prints to phistogram.txt
    histogram(labels, percentages, num_labels, 50);
    fflush(stdout);

    // Restore original stdout
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        perror("dup2 failed");
        exit(1);
    }
    close(saved_stdout);

    // Free allocated memory
    free(labels);
    free(percentages);
    free(matches);
    return 0;
}