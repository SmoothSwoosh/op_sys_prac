#include <stdio.h>
#include <fcntl.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum { SIZE = 1024 };

int 
main(int argc, char *argv[])
{
    unsigned long long amount = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat buffer = {};
        if (lstat(argv[i], &buffer) == 0) {
            unsigned long long file_size = buffer.st_size;
            unsigned long long name_times = buffer.st_nlink;
            if ((file_size % SIZE) == 0 && name_times == 1 && S_ISREG(buffer.st_mode)) {
                amount += file_size;
            }
        }
    }
    
    printf("%llu\n", amount);
    
    return 0;
}