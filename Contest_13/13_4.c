#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

enum
{
    BASE = 10,
    FIRST_ARG = 1,
    SECOND_ARG = 2,
    THIRD_ARG = 3,
    FOURTH_ARG = 4,
    INITIAL_SIZE = 100
};

int 
main(int argc, char *argv[])
{
    double left = strtod(argv[FIRST_ARG], NULL);
    double right = strtod(argv[SECOND_ARG], NULL);
    int n = strtol(argv[THIRD_ARG], NULL, BASE);
    
    const char *format = 
        "#include <math.h>\n"
        "double func(double x) {\n"
        "    return %s;\n"
        "}";
    
    int code_size = strlen(format) + strlen(argv[FOURTH_ARG]) + 1;
    char *library_code = (char *)calloc(code_size, sizeof(char));
    
    if (snprintf(library_code, code_size, format, argv[FOURTH_ARG]) >= code_size) {
        return 1;
    }
    
    unsigned long long number = 0;
    int fd = open("/dev/urandom", O_RDONLY);
    
    if (read(fd, &number, sizeof(number)) != sizeof(number)) {
        return 1;
    }
    
    char *library_name = (char *)calloc(INITIAL_SIZE, sizeof(char)); 
    
    if (snprintf(library_name, INITIAL_SIZE, "./library-%llu.so", number) >= INITIAL_SIZE) {
        return 1;
    }
    
    if (read(fd, &number, sizeof(number)) != sizeof(number)) {
        return 1;
    }
    
    close(fd);
    
    char *file_name = (char *)calloc(INITIAL_SIZE, sizeof(char));
    
    if (snprintf(file_name, INITIAL_SIZE, "./file-%llu.c", number) >= INITIAL_SIZE) {
        return 1;
    }
    
    fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0700);
    
    if (write(fd, library_code, strlen(library_code)) != strlen(library_code)) {
        return 1;
    }
    
    close(fd);

    if (!fork()) {
        //son: library preparation
        execlp("gcc", "gcc", "-shared", "-o", library_name, "-fPIC", file_name, "-lm", NULL);
        _exit(1);
    }
    
    wait(NULL);
    
    void *lib_handler = dlopen(library_name, RTLD_LAZY);
    
    if (lib_handler == NULL) {
        return 1;
    }
    
    double (*func)(double x); //handler for func
    
    func = dlsym(lib_handler, "func");
    
    if (func == NULL) {
        return 1;
    }
    
    double square = 0;
    double dx = (right - left) / n;
    
    for (int i = 0; i < n; ++i) {
        double x = left + i * dx;
        square += func(x) * dx;
    }
    
    printf("%.10g\n", square);
    fflush(stdout);
    
    if (dlclose(lib_handler) != 0) {
        return 1;
    }
    
    free(library_code);
    free(library_name);
    free(file_name);

    return 0;
}