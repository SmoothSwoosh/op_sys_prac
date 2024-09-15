#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

enum
{
    FIRST_ARG = 1,
    SECOND_ARG = 2,
    THIRD_ARG = 3,
    FOURTH_ARG = 4,
    EXIT_CODE = 42
};

int 
main(int argc, char *argv[])
{
    int pid = fork(); //make a child
    
    if (pid < 0) {
        return 1;
    }
    
    if (pid == 0) {
        //child
        int fd_file1, fd_file2, fd_file3;
        if ((fd_file1 = open(argv[SECOND_ARG], O_RDONLY)) == -1) {
            _exit(EXIT_CODE);
        } 
        if ((fd_file2 = open(argv[THIRD_ARG], O_WRONLY | O_APPEND | O_CREAT, 0660)) == -1) {
            _exit(EXIT_CODE);
        } 
        if ((fd_file3 = open(argv[FOURTH_ARG], O_WRONLY | O_TRUNC | O_CREAT, 0660)) == -1) {
            _exit(EXIT_CODE);
        }
        
        if (dup2(fd_file1, 0) == -1) _exit(EXIT_CODE); //redirect input
        if (dup2(fd_file2, 1) == -1) _exit(EXIT_CODE); //redirect output
        if (dup2(fd_file3, 2) == -1) _exit(EXIT_CODE); //redirect error
        
        if (execlp(argv[FIRST_ARG], argv[FIRST_ARG], NULL) == -1) _exit(EXIT_CODE);

        _exit(0);
    } else {
        //parent
        int status = 0;
        if (wait(&status) == -1) return 1;
        printf("%d\n", status);
    }
    
    return 0;
}