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
    
    int n = 0, counter = 1, pid = 0;
    scanf("%d", &n);
    
    while (true) {
        if (counter == n) {
            printf("%d\n", counter);
            exit(0);
        }
        //we're in a child of current process now
        printf("%d ", counter); //output counter
        ++counter; //increment counter for our child
        
        //we're about to create our child
        if (!pid) {
            pid = fork(); //creating a child
        }
        
        if (pid) {
            //current process
            wait(NULL); //is waiting for his child
            exit(0);
        }
    }
    
    return 0;
}