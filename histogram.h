#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

double find_max(double *x, int n);
int find_star(double num, int width, double max);
void histogram(int *x, double *y, int n, int width);
int *histogram_lengths(char **strings, int n);

#endif 
