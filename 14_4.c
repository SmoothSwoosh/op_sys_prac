#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/wait.h>

enum
{
    KEY = 124578,
    BASE = 10
};

void
work(int semid, int index, int count)
{
    while (1) {
        int r = semop(semid, (struct sembuf[])
                {
                    { .sem_num = index, .sem_op = -1 }
                }, 1);
        
        if (r < 0) {
            _exit(1);
        }
        
        int number = 0;
        if (scanf("%d", &number) != 1) {
            semctl(semid, 0, IPC_RMID);
            break;
        }
        
        printf("%d %d\n", index, number);
        fflush(stdout);
        
        int next = (number % count + count) % count;
        
        r = semop(semid, (struct sembuf[])
                {
                    { .sem_num = next, .sem_op = 1 }
                }, 1);
        
        if (r < 0) {
            _exit(1);
        }
    }
}

int 
main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    
    int count = strtol(argv[1], NULL, BASE);
    
    short *sems = (short *)calloc(count, sizeof(sems[0]));
    int semid = semget(KEY, count, 0666 | IPC_CREAT);
    if (semid < 0) {
        return 1;
    }

    semctl(semid, 0, SETALL, sems);
    
    int r = semop(semid, (struct sembuf[])
            {
                { .sem_num = 0, .sem_op = 1 }
            }, 1);
            
    if (r < 0) {
        return 1;
    }
                
    for (int i = 0; i < count; ++i) {
        if (!fork()) {
            work(semid, i, count);
            _exit(0);
        }
    }
    
    while (wait(NULL) > 0) {}
    
    return 0;
}