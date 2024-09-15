#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

enum
{
    FIRST_ARG = 1,
    BASE = 10
};

volatile int flag; 

void
handler(int s)
{
    flag = 1;
}

void
play_numberball(int serial, FILE *fdr, FILE *fdw, int border)
{
    while (1) {
        flag = 0; 
        sigset_t sset;
        sigemptyset(&sset);
        while (!flag) {
            sigsuspend(&sset);
        } 
        
        int number = 0, brother_pid = 0;
        if (fscanf(fdr, "%d%d", &brother_pid, &number) != 2 
                || number >= border) {
            fprintf(fdw, "%d %d\n", getpid(), number);
            fflush(fdw);
            if (kill(brother_pid, SIGUSR1) == -1) _exit(1);
            break;
        }
        
        printf("%d %d\n", serial, number);
        fflush(stdout);
        
        ++number;
        fprintf(fdw, "%d %d\n", getpid(), number);
        fflush(fdw);
        if (kill(brother_pid, SIGUSR1) == -1) _exit(1);
    }
    
    fclose(fdr);
    fclose(fdw);
}

int 
main(int argc, char *argv[])
{
    int border = strtol(argv[FIRST_ARG], NULL, BASE);

    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    
    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sset, NULL);

    int pfd[2];
    if (pipe(pfd) == -1) return 1;
    
    FILE *fdr = fdopen(pfd[0], "r");
    if (fdr == NULL) return 1;
    FILE *fdw = fdopen(pfd[1], "w");
    if (fdw == NULL) return 1;
    setbuf(fdw, NULL);

    int pid1 = fork();
    if (!pid1) {
        //First son
        play_numberball(1, fdr, fdw, border);
        _exit(0);
    }
    
    int pid2 = fork();
    if (!pid2) {
        //Second son
        play_numberball(2, fdr, fdw, border);
        _exit(0);
    }
    
    fprintf(fdw, "%d %d\n", pid2, 1);
    fflush(fdw);
    if (kill(pid1, SIGUSR1) == -1) return 1;
    
    fclose(fdr);
    fclose(fdw);
    
    while (wait(NULL) > 0) {} 
    
    printf("Done\n");
    fflush(stdout);

    return 0;
}