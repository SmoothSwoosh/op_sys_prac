#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


enum
{
    YEAR_BORDER = 1900
};

int 
main(int argc, char *argv[])
{
    int pfd[2];
    pipe(pfd);
    
    if (!fork()) {
        
        if (!fork()) {
            
            if (!fork()) {
                close(pfd[0]);
                struct tm *worm;   
            
                time_t cur_time = time(NULL);
                
                worm = localtime(&cur_time);
                
                int cur_day = worm->tm_mday;
                int cur_month = worm->tm_mon;
                int cur_year = worm->tm_year;
 
                write(pfd[1], &cur_day, sizeof(cur_day));
                write(pfd[1], &cur_month, sizeof(cur_month));
                write(pfd[1], &cur_year, sizeof(cur_year));
               
                close(pfd[1]);
                _exit(0);
            }

            close(pfd[1]);
            
            wait(NULL);
            int cur_day = 0;
            read(pfd[0], &cur_day, sizeof(cur_day));

            printf("D:%d\n", cur_day);
            fflush(stdout);
            
            close(pfd[0]);
            _exit(0);
        }
        
        close(pfd[1]);
        wait(NULL);
        int cur_month = 0;
        read(pfd[0], &cur_month, sizeof(cur_month));
        printf("M:%.02d\n", cur_month + 1);
        fflush(stdout);
            
        close(pfd[0]);
        _exit(0);
    }
    
    close(pfd[1]);
    wait(NULL);
    int cur_year = 0;
        
    read(pfd[0], &cur_year, sizeof(cur_year));
            
    printf("Y:%d\n", cur_year + YEAR_BORDER);
    fflush(stdout);
            
    close(pfd[0]);

    return 0;
}