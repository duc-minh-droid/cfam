#include <float.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

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
    int field_width = get_max_width(x, n);
    for (int i = 0; i < n; i++) {
        printf("%*d | ", field_width, x[i]);  // Align index
        int stars = (int)(y[i] * width / 10.0); // 1 star per 0.2% (adjustable)
        if (stars > width) stars = width; // Cap at max width
        if (y[i] > 0 && stars == 0) stars = 1; // Ensure at least 1 star for non-zero
        for (int j = 0; j < stars; j++) {
            printf("*");
        }
        printf(" %.2f%%\n", y[i]);
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