#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int contains(int *x, int num, int n) {
    for (int i = 0; i < n; i++)
        if (x[i] == num) return 1;
    return 0;
}

int get_index(int *x, int num, int n) {
    for (int i = 0; i < n; i++)
        if (x[i] == num) return i;
    return -1;
}

int get_file_size(char *file_path) {
    int size = 0;
    FILE *fptr = fopen(file_path, "r");
    if (!fptr) {
        perror("Error opening file");
        return -1;
    }
    char buffer[255];
    while (fgets(buffer, 255, fptr)) size++;
    fclose(fptr);
    return size;
}

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
    for (int i = 0; i < size; i++) indexes[i] = i;
    return indexes;
}

char **read_txt_file(char *file_path, int size) {
    if (size <= 0) {
        fprintf(stderr, "Error: File is empty or invalid size.\n");
        return NULL;
    }
    char **words = malloc(size * sizeof(char *));
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
        buffer[strcspn(buffer, "\n")] = '\0';
        words[i] = strdup(buffer);
        if (!words[i]) {
            perror("Memory allocation failed for line");
            while (i--) free(words[i]);
            free(words);
            fclose(fptr);
            return NULL;
        }
    }
    fclose(fptr);
    return words;
}

void free_words(char **words, int size) {
    if (words) {
        for (int i = 0; i < size; i++) free(words[i]);
        free(words);
    }
}