#if !defined(SHUFFLE_H)
#define SHUFFLE_H
#include <gsl/gsl_rng.h>
/* #include <gsl/gsl_randist.h>
*/

gsl_rng *shuffle(int *, int, int);
void free_shuffle(gsl_rng *r);

#endif
