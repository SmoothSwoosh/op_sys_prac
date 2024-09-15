#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

enum
{
    MAX_SIGNALS = 4
};

volatile int last_prime = 0;

int
is_prime(int number)
{
    if (number == 1) {
        return 0;
    }    

    for (int i = 2; i <= sqrt(number); ++i) {
        if (number % i == 0) {
            return 0;
        }
    }
    
    return 1;
}

void
handler_int(int s)
{
    static int sig_counter = 0;
    ++sig_counter;
    if (sig_counter == MAX_SIGNALS) {
        _exit(0);
    }
    printf("%d\n", last_prime);
    fflush(stdout);
}

void
handler_term(int s)
{
    _exit(0);
}

int 
main(int argc, char *argv[])
{
    int low = 0, high = 0;
    scanf("%d%d", &low, &high);

    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler_int, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handler_term, .sa_flags = SA_RESTART }, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    for (int i = low; i < high; ++i) {
        if (is_prime(i)) {
            last_prime = i;
        }
    }

    sigaction(SIGINT, &(struct sigaction) { .sa_handler = SIG_IGN, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = SIG_IGN, .sa_flags = SA_RESTART }, NULL);
    
    printf("-1\n");
    fflush(stdout);

    return 0;
}