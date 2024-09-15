#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <limits.h>

int 
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    
    if (fd == -1) {
        return 1;
    }
    
    long long number = 0, min_number = 0, index = -1, cnt = -1;
    while (read(fd, &number, sizeof(number)) == sizeof(number)) {
        ++cnt;
        if ((number < min_number) || (index == -1)) {
            index = cnt;
            min_number = number;
        }
    }
    
    if (index == -1) {
        close(fd);
        return 0;
    }
    
    if (lseek(fd, index * sizeof(min_number), SEEK_SET) == -1) {
        return 1;
    }
    
    if (min_number != LLONG_MIN) {
        min_number = -min_number;
    }
    
    if (write(fd, &min_number, sizeof(min_number)) != sizeof(min_number)) {
        return 1;
    }

    close(fd);

    return 0;
}