#include <stdio.h>
#include <stdlib.h>

enum
{
    MODULUS = 1U << 31,
    MULTIPLIER = 1103515245,
    INCREMENT = 12345
};

typedef struct RandomGenerator RandomGenerator;

typedef struct RandomOperations
{
    void (*destroy)(RandomGenerator *generator);
    int (*next)(RandomGenerator *generator);
} RandomOperations;

typedef struct RandomGenerator 
{
    const RandomOperations *ops;
    int seed;
} RandomGenerator;

static void
destroy(RandomGenerator *generator)
{
    if (generator) {
        free(generator);
    }
    return;
}

static int
next(RandomGenerator *generator)
{
    unsigned int current_seed = generator->seed;
    generator->seed = (MULTIPLIER * current_seed + INCREMENT) % MODULUS;
    return generator->seed;
}

static const RandomOperations ops = { destroy, next };

RandomGenerator *
random_create(int seed)
{
    RandomGenerator *generator = (RandomGenerator *)calloc(1, sizeof(*generator));
    if (!generator) {
        return NULL;
    }
    generator->seed = seed;
    generator->ops = &ops;
    
    return generator;
}