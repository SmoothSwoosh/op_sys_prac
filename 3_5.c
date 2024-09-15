#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

enum 
{ 
    BUF_SIZE = 50000, 
    ARR_SIZE = 10 
};

int 
main(int argc, char *argv[])
{
    //read from file
    char file_name[PATH_MAX] = {0};

    fgets(file_name, sizeof(file_name), stdin);

    int name_length = strlen(file_name);
    
    //check for end of the line
    for (int i = 0; i < name_length; ++i) {
        if (file_name[i] == '\n' || file_name[i] == '\r') {
            file_name[i] = '\0';
            break;
        }
    }
    
    unsigned long long entries[ARR_SIZE] = {0}; //answer array
    
    if (file_name[0] != '\0') {
        int fd = open(file_name, O_RDONLY);
        if (fd >= 0) {
            char *buf = calloc(BUF_SIZE, 1); //buffer
            int cnt_read = 0;
            while ((cnt_read = read(fd, &buf[0], BUF_SIZE)) > 0) {
                for (int i = 0; i < cnt_read; ++i) {
                    if (buf[i] >= '0' && buf[i] <= '9') {
                        ++entries[buf[i] - '0']; 
                    }
                }
            }
            free(buf);
            close(fd);
        }
    }
    
    //output answer
    for (int i = 0; i < ARR_SIZE; ++i) {
        printf("%d %llu\n", i, entries[i]);
    }

    return 0;
}