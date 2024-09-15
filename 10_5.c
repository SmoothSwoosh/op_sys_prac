#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
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

    unsigned long long random_value1 = 0, random_value2 = 0;
    if (read(fd, &random_value1, sizeof(random_value1)) != sizeof(random_value1)) {
        return 1;
    }
    
    if (read(fd, &random_value2, sizeof(random_value2)) != sizeof(random_value2)) {
        return 1;
    }

    close(fd);
    
    char *filename = (char *)calloc(INITIAL_SIZE, sizeof(char));
    char *tmp = (char *)calloc(INITIAL_SIZE, sizeof(char));

    if (getenv("XDG_RUNTIME_DIR")) {
        if (snprintf(filename, PATH_MAX, "%s/%s-%llu", getenv("XDG_RUNTIME_DIR"), "task", random_value1)
                >= PATH_MAX) {
            return 1;        
        }
        if (snprintf(tmp, PATH_MAX, "%s/%s-%llu.c", getenv("XDG_RUNTIME_DIR"), "task", random_value2)
                >= PATH_MAX) {
            return 1;        
        }
    } else if (getenv("TMPDIR")) {
        if (snprintf(filename, PATH_MAX, "%s/%s-%llu", getenv("TMPDIR"), "task", random_value1)
                >= PATH_MAX) {
            return 1;        
        }
        if (snprintf(filename, PATH_MAX, "%s/%s-%llu.c", getenv("TMPDIR"), "task", random_value2)
                >= PATH_MAX) {
            return 1;        
        }
    } else {
        if (snprintf(filename, PATH_MAX, "%s/%s-%llu", "/tmp", "task", random_value1)
                >= PATH_MAX) {
            return 1;        
        }
        if (snprintf(tmp, PATH_MAX, "%s/%s-%llu.c", "/tmp", "task", random_value2)
                >= PATH_MAX) {
            return 1;        
        }
    }
    
    fd = open(tmp, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    if (fd < 0) {
        return 1;
    }
    
    FILE *f = fdopen(fd, "w");
    if (f == NULL) {
        return 1;
    }
    
    if (fprintf(f, "%s%s%s", "#include <stdio.h>\n"
        "int main(int argc, char *argv[]){\n"
        "int x = 0;\n"
        "char *summon = \"summon\";"
        "char *disqualify = \"disqualify\";"
        "char *reject = \"reject\";"
        "while (scanf(\"%d\", &x) != -1) {"
        "char *worm = ", argv[FIRST_ARG], ";"
        "printf(\"%s\\n\", worm);}"
        "remove(argv[0]);"
        "remove(argv[1]);"
        "return 0;"
        "}") < 0) {
        return 1;    
    }
    
    fclose(f);
    
    int pid = fork();
    if (pid < 0) {
        // error
        return 1;
    } else if (pid == 0) {
        execlp("gcc", "gcc", tmp, "-o", filename, NULL);
        _exit(1);
    } else {
        // parent
        int status = 0;
        wait(&status);
        execlp(filename, filename, tmp, NULL);
    }

    return 0;
}