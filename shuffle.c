#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

gsl_rng *shuffle(int *x, int n, int seed)
{
    /*
     * Shuffle the n elements of the integer array x in place.
     *
     * Parameters
     * ----------
     *
     * x : array of integers to be shuffled
     *
     * n : length of x
     *
     * seed : integer seed for the random number generator. If seed is a
     *        positive integer, the random number generator is initialised with
     *        that seed on the first call to shuffle; the seed is ignored on all
     *        other calls. If seed is negative, the generator is initialised
     *        with the time so different random number sequences and therefore
     *        shuffles are produced each time the program is run.
     *
     * Returns
     * -------
     *
     *  r : gsl_rng *r;  A pointer to the random number generator's internal 
     *      state.  Can be passed to shuffle_free() to free the memory used 
     *      when no longer requited. 
     *
     * Your program will need to be linked with -lgsl -lgslcblas -lm
     * For example:
     *
     * gcc myprogram.c shuffle.o -lgsl -lgslcblas -lm
     *
     *
     * This is just a convenient wrapper of the GSL gsl_ran_shuffle.
     */

    const gsl_rng_type *T;
    static gsl_rng *r = NULL;

    if (r == NULL) {
        /* Create a random number generator */
        gsl_rng_env_setup();
        if (seed < 0)
            seed = time(NULL);
        gsl_rng_default_seed = seed;

        T = gsl_rng_default;
        r = gsl_rng_alloc (T);
    }

    gsl_ran_shuffle(r, x, n, sizeof(int));
    return r;
}


void free_shuffle(gsl_rng *r)
{
    /* 
     *  Free the memeroy allocted by the suffle radnom number generator.
     *  
     *   Parameters
     *   ----------
     *
     *   r : pointer to a gsl_rng structure returned by shuuffle().
     */

     free(r->state);
     free(r);
     return;
}

