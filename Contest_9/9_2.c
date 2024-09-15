#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int 
main(int argc, char *argv[])
{
    int child_status;
    int pid_child = fork();
    
    if (pid_child < 0) {
        return 1;
    } 
    
    if (!pid_child) {
        int pid_grandson = fork();
        int grandson_status;
        
        if (pid_grandson < 0) {
            return 1;
        }
        
        if (!pid_grandson) {
            printf("3 ");
            exit(0);
        }
        
        wait(&grandson_status);
        if (WIFEXITED(grandson_status)) {
            printf("2 ");
            exit(0);
        } 
    }
    
    wait(&child_status);
    if (WIFEXITED(child_status)) {
        printf("1\n");
    }
    
    return 0;
}
