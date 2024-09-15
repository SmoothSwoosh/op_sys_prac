#include <stdio.h>
#include <sys/signalfd.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void
work1(void)
{
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1);
    sigaddset(&ss, SIGUSR2);
    sigaddset(&ss, SIGIO);
    
    int sfd = signalfd(-1, &ss, 0);
    struct signalfd_siginfo sinfo;
    
    int flag = 1;
    while (flag) {
        unsigned char c = 0;
        for (int i = 0; i < CHAR_BIT; ++i) {
            read(sfd, &sinfo, sizeof(sinfo));
            if (sinfo.ssi_signo == SIGUSR1) {
                kill(sinfo.ssi_pid, SIGALRM);
            } else if (sinfo.ssi_signo == SIGUSR2) {
                c |= (1 << i);
                kill(sinfo.ssi_pid, SIGALRM);
            } else {
                flag = 0;
                break;
            }   
        }
        if (flag == 0) break;
        printf("%c", c);
        fflush(stdout);
    }
    
    close(sfd);
}

void
work2(int brother_pid, char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) _exit(1);
    
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGALRM);
    
    int sfd = signalfd(-1, &ss, 0);
    struct signalfd_siginfo sinfo;
    
    int c1;
    while ((c1 = fgetc(f)) != EOF) {
        unsigned char c = (unsigned char)c1;
        for (int i = 0; i < CHAR_BIT; ++i) {
            if (c & 1) {
                kill(brother_pid, SIGUSR2);
                read(sfd, &sinfo, sizeof(sinfo));
            } else {
                kill(brother_pid, SIGUSR1);
                read(sfd, &sinfo, sizeof(sinfo));
            }
            c >>= 1;
        }
    }
    
    fclose(f);
    close(sfd);
    
    kill(brother_pid, SIGIO);
}

int 
main(int argc, char *argv[])
{
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1);
    sigaddset(&ss, SIGUSR2);
    sigaddset(&ss, SIGIO);
    sigaddset(&ss, SIGALRM);
    sigprocmask(SIG_BLOCK, &ss, NULL);
    
    int pid1 = fork();
    if (!pid1) {
        work1();
        _exit(0);
    }
    
    int pid2 = fork();
    if (!pid2) {
        work2(pid1, argv[1]);
        _exit(0);
    }
    
    while (wait(NULL) > 0) {}

    return 0;
}