#ifndef UTILS_H
#define UTILS_H

/**
 * Checks if an array contains a number.
 * @param x Array to search.
 * @param num Number to find.
 * @param n Array size.
 * @return 1 if found, 0 otherwise.
 */
int contains(int *x, int num, int n);

/**
 * Creates an array of indices (0 to size-1).
 * @param H Unused histogram array.
 * @param size Size of the array.
 * @return Array of indices, or NULL on error.
 */
int *get_indexes(double *H, int size);

/**
 * Finds the index of a number in an array.
 * @param x Array to search.
 * @param num Number to find.
 * @param n Array size.
 * @return Index if found, -1 otherwise.
 */
int get_index(int *x, int num, int n);

/**
 * Counts lines in a file.
 * @param file_path Path to the file.
 * @return Number of lines, or -1 on error.
 */
int get_file_size(const char *file_path);

/**
 * Reads a text file into an array of strings.
 * @param file_path Path to the file.
 * @param size Number of lines.
 * @return Array of strings, or NULL on error.
 */
char **read_txt_file(const char *file_path, int size);

/**
 * Frees an array of strings.
 * @param words Array to free.
 * @param size Number of strings.
 */
void free_words(char **words, int size);

#endif