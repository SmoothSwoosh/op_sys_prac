#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double ROUND = 10000.0;

enum 
{ 
    MAX_PERCENT = 100
};

int 
main(int argc, char *argv[])
{
    double rate = 0.0, percent = 0.0;
    int i = 1;
    rate = strtod(argv[i], NULL);
    for (i = 2; i < argc; ++i) {
        percent = strtod(argv[i], NULL);
        rate += rate * percent / MAX_PERCENT;
        rate = round(ROUND * rate) / ROUND;// for rounding
    }

    printf("%.4f\n", rate);

    return 0;
}