#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    FIRST_ARG = 1,
    SECOND_ARG = 2,
    THIRD_ARG = 3
};

int 
process(const char *str)
{
    int pid = fork();
    
    if (pid < 0) {
        return 0;
    } else if (!pid) {
        //child
        execlp(str, str, NULL);
        _exit(1);
    } else {
        //parent
        int status = 0;
        waitpid(pid, &status, 0);
        return (WIFEXITED(status) && !WEXITSTATUS(status));
    }
}

int 
main(int argc, char *argv[])
{
    const char *cmd1 = argv[FIRST_ARG];
    const char *cmd2 = argv[SECOND_ARG];
    const char *cmd3 = argv[THIRD_ARG];
    
    return !((process(cmd1) || process(cmd2)) && process(cmd3));
}