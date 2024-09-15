#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>

enum
{
    BASE = 10
};

typedef struct Message 
{
    long mtype;
    long long buf[2];
} Message;

void
work(int msqid, int index, int maxval, int nproc)
{
    Message msg = { };
    
    while (1) {
        if (msgrcv(msqid, &msg, sizeof(msg), index, 0) == -1) {
            _exit(1);
        }
        
        long long x3 = msg.buf[0] + msg.buf[1];
        
        printf("%d %lld\n", index - 1, x3);
        fflush(stdout);
        
        if (llabs(x3) > llabs(maxval)) {
            if (msgctl(msqid, IPC_RMID, NULL) == -1) {
                _exit(1);
            }
            break;
        }

        msg.mtype = x3 % nproc + 1;
        msg.buf[0] = msg.buf[1];
        msg.buf[1] = x3;
        if (msgsnd(msqid, &msg, sizeof(msg), 0) == -1) {
            _exit(1);
        }
    }
}

int 
main(int argc, char *argv[])
{
    int key = strtol(argv[1], NULL, BASE);
    int nproc = strtol(argv[2], NULL, BASE);
    long long value1 = strtoll(argv[3], NULL, BASE);
    long long value2 = strtoll(argv[4], NULL, BASE);
    long long maxval = strtoll(argv[5], NULL, BASE);
    
    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        return 1;
    }
    
    for (int i = 0; i < nproc; ++i) {
        int pid = fork();
        
        if (pid < 0) {
            msgctl(msqid, IPC_RMID, NULL);
            while (wait(NULL) > 0) {}

            return 1;
        }

        if (!pid) {
            work(msqid, i + 1, maxval, nproc);
            _exit(0);
        }
    }
    
    Message msg = {1, {value1, value2} };
    if (msgsnd(msqid, &msg, sizeof(msg), 0) == -1) {
        return 1;
    }
    
    while (wait(NULL) > 0) {}
    
    return 0;
}