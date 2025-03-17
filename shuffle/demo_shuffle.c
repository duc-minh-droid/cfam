#include <stdio.h>
#include <stdlib.h>
#include "shuffle.h"

int main(int argc, char **argv)
{
    /*
     * Demonstrate shuffle. If run with a positive integer command line
     * argument, the random number generator is seeded with the argument.
     */

    int N = 20;
    int a[N];
    int seed = -1;  /*  Seed with time */

    if (argc > 1)
        seed = atoi(argv[1]);

    for (int i = 0; i < N; i++)
        a[i] = i;

    shuffle(a, N, seed);

    for (int i = 0; i < N; i++)
        printf("%d\t%d\n", i, a[i]);

    return 0;

}
