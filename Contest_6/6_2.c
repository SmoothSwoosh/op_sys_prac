#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>

const char template[] = ".exe";

enum
{
    FIRST_ARG = 1,
    REQUESTED_AMOUNT_OF_ARGS = 2,
    TEMPLATE_LENGTH = sizeof(template) - 1
};

int 
main(int argc, char *argv[])
{
    const char *file = argv[FIRST_ARG];
    unsigned long long cnt = 0;
    if (argc == REQUESTED_AMOUNT_OF_ARGS) {
        DIR *stream = opendir(file);
        
        if (stream != NULL) {
            struct dirent *worm;
            while ((worm = readdir(stream)) != NULL) {
                struct stat buf = { };
                char full_path[PATH_MAX] = { 0 };
                if (snprintf(full_path, sizeof(full_path), "%s/%s", file, worm->d_name)
                        < sizeof(full_path)) {
                    if (stat(full_path, &buf) != -1) {
                        int length = strlen(full_path);
                    
                        if (S_ISREG(buf.st_mode) 
                                && !access(full_path, X_OK)
                                && length >= TEMPLATE_LENGTH 
                                && !strcmp(&full_path[length - TEMPLATE_LENGTH], template)) {
                            ++cnt;
                        }   
                    }
                }
            }
            closedir(stream);
        }
    }
    
    printf("%llu\n", cnt);

    return 0;
}