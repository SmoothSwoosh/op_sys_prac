#include <stdio.h>
#include <fcntl.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

int 
main(int argc, char *argv[])
{
    enum { BYTE_RANGE = 256 };
    int fd = open(argv[1], O_RDONLY); // open file
       
    if (fd == -1) {
        return 1;
    } 
    unsigned char num [2];
    unsigned short min_even_number = USHRT_MAX, number = 0;
    while (read(fd, num, sizeof(num)) == sizeof(num)) {
        number = (num[0] * BYTE_RANGE) + num[1];
        if ((number % 2 == 0) && (number < min_even_number)) {
            min_even_number = number;
        }
    } 
    
    close(fd);
    
    if (min_even_number == USHRT_MAX) {
        return 0;
    }
    
    printf("%u\n", min_even_number);
    
    return 0;
}