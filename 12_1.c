#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

enum
{
    CALLS_COUNT = 5
};

void 
handler(int s)
{
    static int cnt = 0;
    printf("%d\n", cnt++);
    fflush(stdout);
    if (cnt == CALLS_COUNT) _exit(0);
}

int 
main(int argc, char *argv[])
{
    sigaction(SIGHUP, &(struct sigaction) 
            {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    while (1) { 
        pause(); 
    }

    return 0;
}
