#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

enum
{
    BASE = 10,
    FIRST_ARG = 1,
    FILE_NAMES_INDEX = 2
};

char *
take_name(char *path)
{
    char *file_name = (char *)calloc(PATH_MAX, sizeof(char));
    FILE *f = fopen(path, "r");
    fgets(file_name, PATH_MAX, f);
    file_name[strlen(file_name) - 1] = '\0';
    fclose(f);
    
    return file_name;
}

int 
process(const char *str)
{
    int pid = fork();
    
    if (pid < 0) {
        _exit(1);
    } else if (!pid) {
        //child
        execlp(str, str, NULL);
        _exit(1);
    } else {
        //parent
        int status = 0;
        waitpid(pid, &status, 0);
        return (WIFEXITED(status) && !WEXITSTATUS(status));
    }
}

int 
main(int argc, char *argv[])
{
    int success_proc = 0;
    int n = strtol(argv[FIRST_ARG], NULL, BASE);

    int *pids = (int *)calloc(n, sizeof(int));
    size_t length = 0;
    
    char *file_name;
    
    int i = 0;
    for (i = FILE_NAMES_INDEX; i < argc; ++i) {
        if (i == n + FILE_NAMES_INDEX) break;
        file_name = take_name(argv[i]);
        
        int pid = fork();
        pids[length++] = pid;
        if (!pid) {
            //child
            execlp(file_name, file_name, NULL);
            _exit(1);
        }
    }   
    
    for (int j = 0; j < length; j++) {
        int status = 0;
        waitpid(pids[j], &status, 0);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            ++success_proc;
        }
    }   
    
    for (; i < argc; ++i) {
        file_name = take_name(argv[i]);

        success_proc += process(file_name);
        free(file_name);
    }
    
    printf("%d\n", success_proc);
    fflush(stdout);
    
    free(pids);

    return 0;
}