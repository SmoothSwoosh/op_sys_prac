#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

enum
{
    FIRST_ARG = 1,
    SECOND_ARG = 2,
    THIRD_ARG = 3,
    FOURTH_ARG = 4,
    FIFTH_ARG = 5,
    ERR_CODE = 1
};

int 
main(int argc, char *argv[])
{
    char *cmd1 = argv[FIRST_ARG];
    char *cmd2 = argv[SECOND_ARG];
    char *cmd3 = argv[THIRD_ARG];
    
    int pfd[2];
    if (pipe(pfd) == -1) {
        return 1;
    }
    
    int pid1 = fork();
    if (pid1 < 0) return 1;
    
    if (!pid1) {
        //first son
        if (dup2(pfd[1], 1) == -1) _exit(ERR_CODE);
        close(pfd[0]);
        close(pfd[1]);
        
        if (!fork()) {
            //son of first son
            int fd = open(argv[FOURTH_ARG], O_RDONLY);
            if (fd < 0) _exit(ERR_CODE);
            if (dup2(fd, 0) == -1) _exit(ERR_CODE);
            close(fd);
            execlp(cmd1, cmd1, NULL);
            _exit(ERR_CODE);
        }
        
        int status = 0;
        wait(&status);
        
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            if (!fork()) {
                //second son of first son
                execlp(cmd2, cmd2, NULL);
                _exit(ERR_CODE);
            }
            wait(NULL);
        } else {
            _exit(ERR_CODE);
        }
        _exit(0);
    }
    
    close(pfd[1]);
    
    int pid2 = fork();
    if (pid2 < 0) return 1;
    
    if (!pid2) {
        //second son
        if (dup2(pfd[0], 0) == -1) _exit(ERR_CODE);
        close(pfd[0]);
        
        int fd = open(argv[FIFTH_ARG], O_WRONLY | O_APPEND | O_CREAT, 0600);
        if (fd < 0) _exit(ERR_CODE);
        
        if (dup2(fd, 1) == -1) _exit(ERR_CODE);
        close(fd);
        
        execlp(cmd3, cmd3, NULL);
        _exit(ERR_CODE);
    }
    
    close(pfd[0]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}