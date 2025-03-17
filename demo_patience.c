#include <stdio.h>
#include "patience.h"
#include "shuffle.h"
#include "histogram.h"

int main() {
    int n = 10000; 
    int num_labels = 0;
    int* matches = many_plays(n);
    
    int* labels = get_labels(matches, &num_labels);
    double* percentages = get_percentages(matches, n, num_labels); 
    
    histogram(labels, percentages, num_labels, 30); 
    
    free(labels);
    free(percentages);
    free(matches);
}