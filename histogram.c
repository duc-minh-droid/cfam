#include <float.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/**
 * Computes the number of stars for the histogram bar.
 *
 * @param num Current value.
 * @param width Maximum width of the histogram.
 * @param max Maximum value in the dataset.
 * @return Number of stars.
 */
int find_star(double num, int width, double max)
{
    return (int)ceil((num / max) * width);
}

/**
 * Finds the maximum value in an array of doubles.
 *
 * @param x Array of doubles.
 * @param n Number of elements in the array.
 * @return Maximum value.
 */
double find_max(double *x, int n)
{
    double max = -DBL_MAX;
    double *i = x;
    for (i = x; i < x + n; i++)
    {
        if (*i > max)
        {
            max = *i;
        }
    }
    return max;
}

/**
 * Computes the maximum width required for printing integers.
 *
 * @param x Array of integers.
 * @param n Number of elements in the array.
 * @return Maximum width of printed integers.
 */
int get_max_width(int *x, int n)
{
    int max_width = 0;
    for (int i = 0; i < n; i++)
    {
        int width = snprintf(NULL, 0, "%d", x[i]);
        if (width > max_width)
        {
            max_width = width;
        }
    }
    return max_width;
}

/**
 * Prints a histogram using stars.
 * The index is printed with a fixed field width for alignment.
 *
 * @param x Array of indices.
 * @param y Array of values.
 * @param n Number of elements.
 * @param width Maximum width of the histogram bar.
 */
void histogram(int *x, double *y, int n, int width)
{
    double max = find_max(y, n);
    int field_width = get_max_width(x, n); // Determine field width for indices
    for (int i = 0; i < n; i++)
    {
        // Print index with dynamic field width for alignment.
        printf("%*d ", field_width, x[i]);
        int stars = find_star(y[i], width, max);
        for (int j = 0; j < stars; j++)
        {
            printf("*");
        }
        printf("    %g\n", y[i]);
    }
}

/**
 * Computes frequency counts for string lengths.
 *
 * @param strings Array of strings.
 * @param n Number of strings.
 * @return Array containing frequency counts of string lengths.
 */
int *histogram_lengths(char **strings, int n)
{
    int max_length = 0;
    // Find the maximum string length.
    for (int i = 0; i < n; i++)
    {
        int size = strlen(strings[i]);
        if (size > max_length)
        {
            max_length = size;
        }
    }
    // Allocate memory for frequency counts.
    int *array = (int *)calloc(max_length + 1, sizeof(int));
    if (array == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    // Count frequency of each string length.
    for (int i = 0; i < n; i++)
    {
        int size = strlen(strings[i]);
        array[size]++;
    }
    return array;
}