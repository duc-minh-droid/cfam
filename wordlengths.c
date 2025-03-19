#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "histogram.h"
#include "utils.h"

/**
 * Prints a histogram of word lengths from a file.
 * @param file_path Path to the text file.
 */
void wordlengths(char *file_path) {
    int size = get_file_size(file_path);
    if (size <= 0) {
        fprintf(stderr, "Error: Could not read file or file is empty\n");
        return;
    }

    char **file = read_txt_file(file_path, size);
    if (!file) return;

    int *H = histogram_lengths(file, size);
    if (!H) {
        free_words(file, size);
        return;
    }

    int max_length = 0;
    for (int i = 0; i < size; i++) {  // Find max word length
        int len = strlen(file[i]);
        if (len > max_length) max_length = len;
    }

    double *H_double = malloc((max_length + 1) * sizeof(double));
    if (!H_double) {
        perror("Memory allocation failed");
        free(H);
        free_words(file, size);
        return;
    }

    for (int i = 0; i <= max_length; i++)  // Convert to percentages
        H_double[i] = (H[i] * 100.0) / size;

    int *x = get_indexes(H_double, max_length + 1);
    if (!x) {
        free(H_double);
        free(H);
        free_words(file, size);
        return;
    }

    printf("Word Length Histogram for %s:\n", file_path);
    printf("Length %% Frequency\n");
    histogram(x, H_double, max_length + 1, 50);

    free(x);
    free(H_double);
    free(H);
    free_words(file, size);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
 
    wordlengths(argv[1]);
 
    return 0;
 }