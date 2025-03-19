#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * Checks if a number exists in an array.
 * Loops through the array and returns true (1) if the number is found, false (0) if not.
 * @param x The array to search through.
 * @param num The number to look for.
 * @param n The size of the array.
 * @return 1 if num is in x, 0 otherwise.
 */
int contains(int *x, int num, int n) {
    for (int i = 0; i < n; i++)
        if (x[i] == num) return 1; // Found it!
    return 0; // Not there
}

/**
 * Finds the position of a number in an array.
 * Scans the array and returns the index where the number appears, or -1 if it’s not found.
 * @param x The array to search in.
 * @param num The number to find.
 * @param n The size of the array.
 * @return Index of num in x, or -1 if not present.
 */
int get_index(int *x, int num, int n) {
    for (int i = 0; i < n; i++)
        if (x[i] == num) return i; // Here’s the spot!
    return -1; // Didn’t find it
}

/**
 * Counts the number of lines in a text file, assuming each line is a word or entry.
 * Opens the file, reads line by line, and tallies them up—great for knowing how many
 * words we’ll need to process later.
 * @param file_path Path to the text file.
 * @return Number of lines (size), or -1 if the file can’t be opened.
 */
int get_file_size(const char *file_path) {
    int size = 0;
    FILE *fptr = fopen(file_path, "r");
    if (!fptr) {
        perror("Error opening file");
        return -1;
    }
    char buffer[255];
    while (fgets(buffer, 255, fptr)) size++; // Count each line
    fclose(fptr);
    return size;
}

/**
 * Creates an array of indices (0, 1, 2, ...) for a given size.
 * Useful when you need a simple sequence of numbers, like for indexing another array.
 * @param H Array we’re basing the size on (just used for its length, not its values).
 * @param size Length of the array to create.
 * @return Array of indices, or NULL if allocation fails or input is invalid.
 */
int *get_indexes(double *H, int size) {
    if (!H) {
        fprintf(stderr, "Error: NULL array in get_indexes.\n");
        return NULL;
    }
    int *indexes = malloc(size * sizeof(int));
    if (!indexes) {
        perror("Memory allocation failed");
        return NULL;
    }
    for (int i = 0; i < size; i++) indexes[i] = i; // Fill with 0, 1, 2, ...
    return indexes;
}

/**
 * Reads a text file into an array of strings, one string per line.
 * Allocates memory for each line and copies it in—perfect for loading a word list.
 * Cleans up if anything goes wrong to avoid memory leaks.
 * @param file_path Path to the text file.
 * @param size Number of lines to read (from get_file_size).
 * @return Array of strings, or NULL if something fails.
 */
char **read_txt_file(const char *file_path, int size) {
    if (size <= 0) {
        fprintf(stderr, "Error: File is empty or invalid size.\n");
        return NULL;
    }
    char **words = malloc(size * sizeof(char *)); // Space for pointers to each word
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
    for (int i = 0; i < size && fgets(buffer, 255, fptr); i++) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Strip off the newline
        words[i] = strdup(buffer); // Copy the line into its own memory
        if (!words[i]) {
            perror("Memory allocation failed for line");
            while (i--) free(words[i]); // Free what we’ve got so far
            free(words);
            fclose(fptr);
            return NULL;
        }
    }
    fclose(fptr);
    return words;
}

/**
 * Frees an array of strings and all its contents.
 * Safely cleans up memory allocated by read_txt_file to prevent leaks.
 * @param words Array of strings to free.
 * @param size Number of strings in the array.
 */
void free_words(char **words, int size) {
    if (words) { // Only proceed if there’s something to free
        for (int i = 0; i < size; i++) free(words[i]); // Free each string
        free(words); // Free the array of pointers
    }
}