#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>

enum
{
    FIRST_COMMAND_ARG = 1,
    SECOND_COMMAND_ARG = 2,
    THIRD_COMMAND_ARG = 3,
    FOURTH_COMMAND_ARG = 4,
    FIFTH_COMMAND_ARG = 5,
    BASE = 10,
    MICRO_IN_SEC = 1000000
};

int
main(int argc, char *argv[])
{
    struct timeval pval = { }; //structure timeval to represent time
    double stream_par = 0.0; //stream parameter
    unsigned int seed = 0; //seed for srand
    int k = 0; //number of events
    pval.tv_sec = strtol(argv[FIRST_COMMAND_ARG], NULL, BASE); 
    pval.tv_usec = strtol(argv[SECOND_COMMAND_ARG], NULL, BASE);
    stream_par = strtod(argv[THIRD_COMMAND_ARG], NULL);
    k = strtol(argv[FOURTH_COMMAND_ARG], NULL, BASE);
    seed = strtol(argv[FIFTH_COMMAND_ARG], NULL, BASE);
    srand(seed);
    
    for (size_t i = 0; i < k; ++i) {
        double u = (double) rand() / RAND_MAX; //generate random value
        pval.tv_usec += (long)(-log(u) / stream_par); //add msecs to current
        pval.tv_sec += pval.tv_usec / MICRO_IN_SEC;
        pval.tv_usec %= MICRO_IN_SEC;
        printf("%ld %ld\n", pval.tv_sec, pval.tv_usec); //output
    }    
    
    return 0;
}   