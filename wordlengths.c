#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include "histogram.h"

int get_file_size(char *file_path) {
  int size = 0;
    FILE *fptr;
    fptr = fopen(file_path, "r");
    if (!fptr) {
        perror("Error opening file");
        return -1;
    }
    char buffer[255];
    while (fgets(buffer, 255, fptr) != NULL) {
        size++;
    }
    fclose(fptr);
    return size;
}

char **read_txt_file(char *file_path, int size) {
    if (size <= 0) {
        fprintf(stderr, "Error: File is empty or could not be read.\n");
        return NULL;
    }
    char **words = (char **)malloc(size * sizeof(char *));
    if (!words) {
        perror("Memory allocation failed");
        return NULL;
    }

    FILE *fptr = fopen(file_path, "r");
    if (!fptr) {
        perror("Error opening file");
        free(words);
        return NULL;
    }
    char buffer[255];
    int i = 0;
    while (fgets(buffer, 255, fptr) != NULL && i < size) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline
        words[i] = strdup(buffer);  // Store each line dynamically
        if (!words[i]) {
            perror("Memory allocation failed for line");
            break;
        }
        i++;
    }
    fclose(fptr);
    return words;
}

int *get_indexes(double *H, int size) {
    if (!H) {
        fprintf(stderr, "Error: NULL array passed to get_indexes.\n");
        return NULL;
    }

    int *indexes = (int *)malloc(size * sizeof(int));
    if (!indexes) {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        indexes[i] = i;
    }

    return indexes;
}

//int find_max_width(int *lengths, int size) {
//    if (!lengths || size <= 0) {
//        fprintf(stderr, "Error: Invalid array or size passed to find_max_width.\n");
//        return -1;  // Return an error value
//    }
//
//    int max = INT_MIN;  // Start with the smallest possible integer
//    for (int i = 0; i < size; i++) {
//        if (lengths[i] > max) {
//            max = lengths[i];
//        }
//    }
//    return max;
//}

void free_words(char **words, int size) {
    if (words) {
        for (int i = 0; i < size; i++) {
            if (words[i]) {
                free(words[i]);
            }
        }
        free(words);
    }
}

void wordlengths(char *file_path) {

    // Read file and get word lengths
    int size = get_file_size(file_path);
    if (size <= 0) {
        fprintf(stderr, "Error: Could not read file or file is empty\n");
        return;
    }

    char **file = read_txt_file(file_path, size);
    if (!file) {
        return;
    }

    // Get histogram of word lengths
    int *H = histogram_lengths(file, size);
    if (!H) {
        free_words(file, size);
        return;
    }

    // Find the maximum length of any word
    int max_length = 0;
    for (int i = 0; i < size; i++) {
        int len = strlen(file[i]);
        if (len > max_length) {
            max_length = len;
        }
    }

    // Create array of indexes and convert counts to percentages
    double *H_double = (double *)malloc((max_length + 1) * sizeof(double));
    if (!H_double) {
        perror("Memory allocation failed");
        free(H);
        free_words(file, size);
        return;
    }

    // Calculate total words for percentage calculation
    int total_words = size;

    // Convert counts to percentages
    for (int i = 0; i <= max_length; i++) {
        H_double[i] = (H[i] * 100.0) / total_words;  // Convert to percentage
    }

    // Get indexes for the histogram
    int *x = get_indexes(H_double, max_length + 1);
    if (!x) {
        free(H_double);
        free(H);
        free_words(file, size);
        return;
    }

    // Draw the histogram with a width of 50 characters for the longest bar
    printf("Word Length Histogram for %s:\n", file_path);
    printf("Length %% Frequency\n");
    histogram(x, H_double, max_length + 1, 50);

    // Clean up
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