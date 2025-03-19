#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "histogram.h"
#include "utils.h"

/**
 * Prints a histogram showing the distribution of word lengths from a text file.
 * Reads the file, calculates how often each word length appears, and displays a
 * histogram where each bar represents a length’s percentage of the total words.
 * @param file_path Path to the text file containing words, one per line.
 */
void wordlengths(char *file_path) {
    // First, count how many words (lines) are in the file
    int size = get_file_size(file_path);
    if (size <= 0) {
        fprintf(stderr, "Error: Could not read file or file is empty\n");
        return;
    }

    // Load all the words into an array for processing
    char **file = read_txt_file(file_path, size);
    if (!file) return;  // Bail out if we couldn’t read the file

    // Build a histogram of word lengths (e.g., how many 3-letter words, etc.)
    int *H = histogram_lengths(file, size);
    if (!H) {
        free_words(file, size);  // Clean up before exiting if this fails
        return;
    }

    // Find the length of the longest word to set the histogram’s range
    int max_length = 0;
    for (int i = 0; i < size; i++) {
        int len = strlen(file[i]);
        if (len > max_length) max_length = len;
    }

    // Allocate space for percentages and convert counts to percentages
    double *H_double = malloc((max_length + 1) * sizeof(double));
    if (!H_double) {
        perror("Memory allocation failed");
        free(H);
        free_words(file, size);
        return;
    }
    for (int i = 0; i <= max_length; i++) {
        H_double[i] = (H[i] * 100.0) / size;  // Percentage of words with this length
    }

    // Create an array of indices (0, 1, 2, ..., max_length) for the x-axis
    int *x = get_indexes(H_double, max_length + 1);
    if (!x) {
        free(H_double);
        free(H);
        free_words(file, size);
        return;
    }

    // Print the histogram with a nice title and labels
    printf("Word Length Histogram for %s:\n", file_path);
    printf("Length %% Frequency\n");
    histogram(x, H_double, max_length + 1, 50);  // 50 sets the bar width

    // Free up all the memory we used to avoid leaks
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