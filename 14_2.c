#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/sem.h>

enum
{
    BASE = 10,
    MODULE = 10
};

void
work(int semid, int *ptr, int iter_count, int count)
{
    int r;
    for (int i = 0; i < iter_count; ++i) {
        int ind1 = rand() % count;
        int ind2 = rand() % count;
        int value = rand() % MODULE;
        
        if (ind1 != ind2) {
            r = semop(semid, (struct sembuf[])
                    {
                        { .sem_num = ind1, .sem_op = -1 },
                        { .sem_num = ind2, .sem_op = -1 }
                    }, 2);
                    
            if (r < 0) {
                _exit(1);
            }
        }
       
        operation(ptr, ind1, ind2, value);
        
        if (ind1 != ind2) {
            r = semop(semid, (struct sembuf[]) 
                    {
                        { .sem_num = ind1, .sem_op = 1 },
                        { .sem_num = ind2, .sem_op = 1 }
                    }, 2);
            
            if (r < 0) {
                _exit(1);
            }
        }
    }
}

int
main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, BASE);
    int key = strtol(argv[2], NULL, BASE);
    int nproc = strtol(argv[3], NULL, BASE);
    int iter_count = strtol(argv[4], NULL, BASE);
    
    int *ptr = mmap(NULL, count * sizeof(ptr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) {
        return 1;
    }
    
    for (int i = 0; i < count; ++i) {
        int number = 0;
        if (scanf("%d", &number) != 1) {
            return 1;
        }
        ptr[i] = number;
    }
    
    short *sems = (short *)calloc(count, sizeof(sems[0]));
    for (int i = 0; i < count; ++i) {
        sems[i] = 1;
    }
    
    int semid = semget(key, count, 0666 | IPC_CREAT);
    if (semid < 0) {
        return 1;
    }
    
    semctl(semid, 0, SETALL, sems);
    
    for (int i = 0; i < nproc; ++i) {
        if (!fork()) {
            int seed = strtol(argv[i + 5], NULL, BASE);
            srand(seed);
            work(semid, ptr, iter_count, count);
            _exit(0);
        }
    }
    
    while (wait(NULL) > 0) {}
    
    semctl(semid, 0, IPC_RMID);
    
    for (int i = 0; i < count; ++i) {
        printf("%d\n", ptr[i]);
        fflush(stdout);
    }
    
    munmap(ptr, count * sizeof(ptr));
    
    return 0;
}