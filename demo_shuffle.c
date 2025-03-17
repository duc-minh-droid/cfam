#include <stdio.h>
#include <stdlib.h>
#include "shuffle.h"

int main(int argc, char **argv)
{
    /*
     * Demonstrate shuffle. If run with a positive integer command line
     * argument, the random number generator is seeded with the argument.
     */

    int i;
    int N = 20;
    int a[N];
    gsl_rng *r;
    int seed = -1;  /*  Seed with time */

    if (argc > 1)
        seed = atoi(argv[1]);

    for (i = 0; i < N; i++)
        a[i] = i;

    r = shuffle(a, N, seed);

    for (i = 0; i < N; i++)
        printf("%d\t%d\n", i, a[i]);

    free_shuffle(r);
    return 0;

}
