#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

enum
{
    CHILD_COUNT = 3,
    STR_LENGTH = 8,
    BASE = 10
};

//read, process and output number
int
process(int index)
{
    char str[STR_LENGTH + 1] = { 0 };
    int bytes_to_read = sizeof(str) - 1;
    
    if (read(0, &str, sizeof(str) - 1) != bytes_to_read) {
        return 1;
    }
    
    int number = strtol(str, NULL, BASE);
    printf("%d %d\n", index, number * number);
    fflush(stdout);
    
    return 0;
}

int 
main(int argc, char *argv[])
{
    int pid = 0;
    int pids[CHILD_COUNT] = { 0 };
    
    for (int index = 1; index <= CHILD_COUNT; ++index) {
        pid = fork();
        
        pids[index - 1] = pid;
        
        if (!pid) {
            if (process(index)) {
                //if read call has been incorrect
                exit(1);
            }   
            exit(0);
        }
    }
    
    for (int i = 0; i < CHILD_COUNT; ++i) {
        if (waitpid(pids[i], NULL, 0) == -1) {
            return 1;
        }
    }
    
    return 0;
}