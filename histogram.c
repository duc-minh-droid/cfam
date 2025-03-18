#include <float.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/**
 * Computes the number of stars for a histogram bar.
 * @param num Current value.
 * @param width Maximum bar width.
 * @param max Maximum value in dataset.
 * @return Number of stars.
 */
int find_star(double num, int width, double max) {
    return (int)ceil((num / max) * width);
}

/**
 * Finds the maximum value in an array.
 * @param x Array of doubles.
 * @param n Number of elements.
 * @return Maximum value.
 */
double find_max(double *x, int n) {
    double max = -DBL_MAX;
    for (int i = 0; i < n; i++)
        if (x[i] > max) max = x[i];
    return max;
}

/**
 * Computes the maximum width for printing integers.
 * @param x Array of integers.
 * @param n Number of elements.
 * @return Maximum width needed.
 */
int get_max_width(int *x, int n) {
    int max_width = 0;
    for (int i = 0; i < n; i++) {
        int width = snprintf(NULL, 0, "%d", x[i]);
        if (width > max_width) max_width = width;
    }
    return max_width;
}

/**
 * Prints a histogram with aligned indices.
 * @param x Array of indices.
 * @param y Array of values.
 * @param n Number of elements.
 * @param width Maximum bar width.
 */
void histogram(int *x, double *y, int n, int width) {
    double max = find_max(y, n);
    int field_width = get_max_width(x, n);
    for (int i = 0; i < n; i++) {
        printf("%*d ", field_width, x[i]);  // Align index
        int stars = find_star(y[i], width, max);
        for (int j = 0; j < stars; j++) printf("*");
        printf("    %g\n", y[i]);
    }
}

/**
 * Computes frequency of string lengths.
 * @param strings Array of strings.
 * @param n Number of strings.
 * @return Frequency array, or NULL on failure.
 */
int *histogram_lengths(char **strings, int n) {
    int max_length = 0;
    for (int i = 0; i < n; i++) {  // Find max length
        int size = strlen(strings[i]);
        if (size > max_length) max_length = size;
    }

    int *array = calloc(max_length + 1, sizeof(int));
    if (!array) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < n; i++)  // Count frequencies
        array[strlen(strings[i])]++;
    return array;
}