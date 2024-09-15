#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

enum
{
    MAX_BITS = 32
};

int32_t
convert_to_sign_magnitude(uint32_t number, uint32_t shift)
{
    uint32_t mask = 1u << shift;
    int32_t converted_number = number & (mask - 1u);

    return (number & mask) ? -converted_number : converted_number;
}

int
main(int argc, char *argv[])
{
    uint32_t n = 0, s = 0, w = 0;
    scanf("%" SCNu32 "%" SCNu32 "%" SCNu32, &n, &s, &w);

    int32_t signed_magnitude = 0;
    uint32_t current_number = 0;

    uint32_t bound = 0;
    if (n == MAX_BITS) {
        bound = UINT_MAX;
    } else {
        bound = (1u << n) - 1u;
    }

    while (current_number <= bound) {
        signed_magnitude = convert_to_sign_magnitude(current_number, n - 1);
        printf("|%*"PRIo32"|%*"PRIu32"|%*"PRId32"|\n", (int)w, current_number, (int)w,
                current_number, (int)w, signed_magnitude);
        if (current_number > (bound - s)) {
            break;
        }
        current_number += s;
    }

    return 0;
}