#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    enum { RADIX = 10 };
    int number = 0;
    long long positive_total = 0, negative_total = 0;
    for (int i = 1; i < argc; ++i) {
        number = strtol(argv[i], 0, RADIX);
        if (number > 0) { 
            positive_total += number;
        } else {
            negative_total += number; 
        }
    }
    
    printf("%lld\n%lld\n", positive_total, negative_total);

    return 0;
}