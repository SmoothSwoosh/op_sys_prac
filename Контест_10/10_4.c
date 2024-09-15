#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

enum
{
    FIRST_ARG = 1,
    INITIAL_SIZE = 100
};

int 
main(int argc, char *argv[])
{
    int fd = open("/dev/urandom", O_RDONLY);
    
    if (fd < 0) {
        return 1;
    }
    
    unsigned long long random_number = 0;
    if (read(fd, &random_number, sizeof(random_number)) != sizeof(random_number)) {
        return 1;
    }
    
    close(fd);
    
    //prepairing a file
    char *filename = (char *)calloc(INITIAL_SIZE, sizeof(char));
    
    if (snprintf(filename, INITIAL_SIZE, "python3-%llu.py", random_number)
            >= INITIAL_SIZE) {
        return 1;        
    }

    char *full_path = (char *)calloc(PATH_MAX, sizeof(char));
    
    if (getenv("XDG_RUNTIME_DIR")) {
        if (snprintf(full_path, PATH_MAX, "%s/%s", getenv("XDG_RUNTIME_DIR"), filename)
                >= PATH_MAX) {
            return 1;        
        }
    } else if (getenv("TMPDIR")) {
        if (snprintf(full_path, PATH_MAX, "%s/%s", getenv("TMPDIR"), filename)
                >= PATH_MAX) {
            return 1;        
        }
    } else {
        if (snprintf(full_path, PATH_MAX, "%s/%s", "/tmp", filename)
                >= PATH_MAX) {
            return 1;        
        }
    }
    
    //open or create file 
    fd = open(full_path, O_WRONLY | O_TRUNC | O_CREAT, 0700);
    
    if (fd < 0) {
        return 1;
    }
    
    FILE *f = fdopen(fd, "w");
    if (f == NULL) {
        return 1;
    }
    
    fprintf(f, "%s", "#! /usr/bin/python3\n"
        "from os import remove\n"
        "print(");
    
    //process an input
    for (size_t i = FIRST_ARG; i < argc; ++i) {      
        if (i == argc - 1) {
            fprintf(f, "%s)\n", argv[i]);
        } else {
            fprintf(f, "%s*\n", argv[i]);
        }
    }
    
    fprintf(f, "%s%s%s", "remove('", full_path, "')\n");

    fclose(f);  
       
    if (execlp(full_path, full_path, NULL) == -1) {
        return 1;
    }
    
    return 0;
}