#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>

enum
{
    FIRST_ARG = 1,
    BASE = 10
};

void
work(int serial, FILE *fdr, FILE *fdw, int border)
{
    while (true) {
        int number = 0;
        if (fscanf(fdr, "%d", &number) != 1
                || number >= border) {
            break;
        }
        printf("%d %d\n", serial, number);
        fflush(stdout);
        ++number;
        fprintf(fdw, "%d\n", number);
        fflush(fdw);
    }
    
    fclose(fdr);
    fclose(fdw);
    return;
}

int 
main(int argc, char *argv[])
{
    int border = strtol(argv[FIRST_ARG], NULL, BASE);
    
    int p1[2];
    int p2[2];
    if (pipe(p1) == -1) return 1;
    if (pipe(p2) == -1) return 1;
    
    FILE *p1_read = fdopen(p1[0], "r");
    if (p1_read == NULL) return 1;
    FILE *p1_write = fdopen(p1[1], "w");
    if (p1_write == NULL) return 1;
    
    FILE *p2_read = fdopen(p2[0], "r");
    if (p2_read == NULL) return 1;
    FILE *p2_write = fdopen(p2[1], "w");
    if (p2_write == NULL) return 1;
    
    if (!fork()) {
        fclose(p1_write);
        fclose(p2_read);
        work(1, p1_read, p2_write, border);
        _exit(0);
    }
    
    if (!fork()) {
        fclose(p1_read);
        fclose(p2_write);
        work(2, p2_read, p1_write, border);
        _exit(0);
    }
    
    int number = 1;
    fprintf(p1_write, "%d\n", number);
    fflush(p1_write);
    
    fclose(p1_write);
    fclose(p2_read);
    fclose(p1_read);
    fclose(p2_write);

    while (wait(NULL) > 0) {}
    
    printf("Done\n");
    fflush(stdout);

    return 0;
}