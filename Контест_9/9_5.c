#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

int 
main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    int number = 0;
    int pid = fork();
    
    if (!pid) {
        //child of main process
        while (true) {
            if (scanf("%d", &number) != 1) {
                //if the end of file is reached
                exit(0); 
            }
            
            pid = fork();
            if (pid == -1) {
                //if the limit of processes is exceeded
                exit(1); //return to parent this message and exit
            } 
            if (pid) {
                //parent
                int status = 0;
                wait(&status); //is waiting for his child
                if (WIFEXITED(status) && (WEXITSTATUS(status) == 1)) {
                    //if the limit of processes has been exceeded
                    exit(1); //return to our parent this message and exit
                } else {
                    printf("%d\n", number); //output our value
                    exit(0); //exit with exit code == 0 (OK)
                }
            }
        }
    } else {
        //main process
        int main_status = 0;
        wait(&main_status);
        if (WIFEXITED(main_status) && (WEXITSTATUS(main_status) == 1)) {
            //if the limit of processes has been exceeded
            printf("-1\n");
            exit(0);
        }
    }
    
    return 0;
}
