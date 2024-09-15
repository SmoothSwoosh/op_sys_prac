#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    ERR_CODE = 127
};

int 
mysys(const char *str)
{
    int pid = fork();
    
    if (pid < 0) {
        //the process couldn't be run
        return -1;
    } else if (!pid) {
        //child
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(ERR_CODE); //if interpreter couldn't be run
    } else {
        //parent
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            return WTERMSIG(status) + 128;
        } else {
            return WEXITSTATUS(status);
        }
    }
}
