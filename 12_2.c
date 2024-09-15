#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

enum
{
    ADD = 0,
    MULTIPLY = 1
};

volatile int flag = ADD;

void
handler_int(int s)
{
    flag = ADD;
}

void 
handler_quit(int s)
{
    flag = MULTIPLY;
}

int 
main(int argc, char *argv[])
{
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler_int, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction) { .sa_handler = handler_quit, .sa_flags = SA_RESTART }, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    int number = 0;
    unsigned int result = 0;
    while (scanf("%d", &number) == 1) {
        if (flag == ADD) {
            result += number;
        } else {
            result *= number;
        }
        printf("%d\n", result);
        fflush(stdout);
    }

    return 0;
}