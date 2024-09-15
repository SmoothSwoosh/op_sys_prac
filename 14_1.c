#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/wait.h>

enum
{
    BASE = 10
};

void
work(int semid, int *ptr, int index, int nproc, int maxval)
{
    while (1) {
        int r = semop(semid, (struct sembuf[])
                {
                    { .sem_num = index, .sem_op = -1 }
                }, 1);
                
        if (r < 0) {
            _exit(1);
        }
        
        int val = (int) *ptr;
        ptr++;
        printf("%d %d %d\n", index + 1, val, (int) *ptr);
        fflush(stdout);
        
        if (val == maxval) {
            semctl(semid, 0, IPC_RMID);
            break;
        }
        
        *ptr = index + 1;
        ptr--;
        *ptr = ++val;
        
        int tmp = val % nproc;
        int next = 1;
        for (int i = 0; i < 4; ++i) {
            next *= tmp;
            next %= nproc;
        }
  
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
    int nproc = strtol(argv[1], NULL, BASE);
    int key = strtol(argv[2], NULL, BASE);
    int maxval = strtol(argv[3], NULL, BASE);
    
    int semid = semget(key, nproc, 0666 | IPC_CREAT);
    if (semid < 0) {
        return 1;
    }
    
    short *sems = (short *)calloc(nproc, sizeof(sems[0]));
    for (int i = 0; i < nproc; ++i) {
        sems[i] = 0;
    }
    
    semctl(semid, 0, SETVAL, sems);
    
    int *ptr = mmap(NULL, 2 * sizeof(ptr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    
    if (ptr == MAP_FAILED) {
        return 1;
    }
    
    int r = semop(semid, (struct sembuf[])
            {
                { .sem_num = 0, .sem_op = 1 }
            }, 1);
    
    if (r < 0) {
        return 1;
    }
    
    for (int i = 0; i < nproc; ++i) {
        if (!fork()) {
            work(semid, ptr, i, nproc, maxval);
            _exit(0);
        }
    }
    
    while (wait(NULL) > 0) {}
    
    munmap(ptr, 2 * sizeof(ptr));
    
    return 0;
}