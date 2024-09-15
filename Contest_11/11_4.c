#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int 
main(int argc, char *argv[])
{
    int *pids = (int *)calloc(argc, sizeof(int));
    size_t length = 0;

    int pfd[2];
    int fd_in = 0;
    
    for (int i = 1; i < argc; ++i) {
        int status = pipe(pfd);
        int pid = fork();
        if (pid < 0 || status == -1) {
            for (int i = 0; i < length; ++i) {
                kill(pids[i], SIGKILL);
                waitpid(pids[i], NULL, 0);
            }
            return 1;
        }
        pids[length++] = pid;
        if (!pid) {
            //son
            if (i != argc - 1) dup2(pfd[1], 1);
            close(pfd[1]);
            close(pfd[0]);
            if (i != 1) {
                dup2(fd_in, 0);
                close(fd_in);
            }
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        } 
        if (i != 1) close(fd_in);
        close(pfd[1]);
        fd_in = pfd[0];
    }
    
    if (fd_in != 0) close(fd_in);
    
    for (int i = 0; i < length; ++i) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}