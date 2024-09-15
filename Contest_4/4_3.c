#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <limits.h>
#include <stdlib.h>

enum { STRING_SIZE = 16, AMOUNT_SIZE = 4, BYTE = 8, GALLEON = 493, SHEKLEY = 29 };

struct Bill 
{
    char name[STRING_SIZE];
    unsigned char amount[AMOUNT_SIZE];
};

int
convertToLittleEndian(unsigned char *arr) 
{
    //make int
    unsigned int little_endian_number = 0;
    for (int i = 0; i < AMOUNT_SIZE; ++i) {
        little_endian_number <<= BYTE;
        little_endian_number |= arr[i];
    }
    
    return little_endian_number;
}

int 
main(int argc, char *argv[])
{
    int fd = 0; //file descriptor
    long long first_max = LLONG_MIN, second_max = LLONG_MIN;
    //lets look at all files from command line
    for (int i = 1; i < argc; ++i) {
        fd = open(argv[i], O_RDONLY); //open file

        if (fd == -1) {
            continue;
        }
        
        struct Bill worm = {0};
        while (read(fd, &worm, sizeof(worm)) == sizeof(worm)) {
            long long check = convertToLittleEndian(worm.amount);
            if (check > first_max) {
                second_max = first_max;
                first_max = check;
            } else if (check > second_max && check < first_max) {
                second_max = check;
            }
        }

        close(fd); //close file
    }

    if (second_max == LLONG_MIN) {
        return 0;
    }

    long long galleons = llabs(second_max) / GALLEON;
    long long sheklies = (llabs(second_max) % GALLEON) / SHEKLEY;
    long long knuts = (llabs(second_max) % GALLEON) % SHEKLEY;
    
    if (second_max < 0) {
        printf("-%lldg%.2llds%.2lldk\n", galleons, sheklies, knuts);
    } else {
        printf("%lldg%.2llds%.2lldk\n", galleons, sheklies, knuts);
    }
    
    return 0;
}