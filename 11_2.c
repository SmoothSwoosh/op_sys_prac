#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int 
main(int argc, char *argv[])
{
    int pfd[2];
    pipe(pfd);
    //parent
    if (!fork()) {
        //son
        if (!fork()) {
            //grandson 
            close(pfd[1]);
            
            int worm = 0;
            long long amount = 0;
            while (read(pfd[0], &worm, sizeof(worm)) == sizeof(worm)) {
                amount += worm;
            }
            
            printf("%lld\n", amount);
            fflush(stdout);
            
            close(pfd[0]);
            _exit(0);
        }
        
        close(pfd[0]);
        close(pfd[1]);

        wait(NULL);
        _exit(0);
    }

    close(pfd[0]);
    int worm = 0;
    while (scanf("%d", &worm) == 1) {
        write(pfd[1], &worm, sizeof(worm));
    }
    close(pfd[1]);   
 
    wait(NULL);


    return 0;
}