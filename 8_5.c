#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

enum
{
    FIRST_ARG = 1,
    REQUESTED_AMOUNT_OF_ARGS = 2
};

int 
comp(const void *a, const void *b)
{
    return strcasecmp(*(char **)a, *(char **)b);
}

void
recursive_traversal(const char *file, bool flag)
{
    DIR *directory = opendir(file);
    
    if (directory == NULL) return;
     
    if (!flag) {
        printf("cd %s\n", strrchr(file, '/') + 1);
    }
    
    struct dirent *worm;
    struct stat buf = { };
    memset(&buf, 0, sizeof(buf));
    
    size_t arr_size = 0;
    size_t catalog_size = 1;
    char **catalogs = (char **)calloc(catalog_size, sizeof(char *));
    
    while ((worm = readdir(directory)) != NULL) {
        char full_path[PATH_MAX] = { 0 };
        if (snprintf(full_path, sizeof(full_path), "%s/%s", file, worm->d_name)
                < sizeof(full_path)) {
            //if length of the path to the file is less than PATH_MAX
            if (lstat(full_path, &buf) != -1) {
                // if we can take an information about the file
                if (S_ISDIR(buf.st_mode)) {
                    //if this is a catalog's file
                    if (strcmp(worm->d_name, ".")
                            && strcmp(worm->d_name, "..")) { 
                        char *path = (char *)
                                calloc(PATH_MAX, sizeof(char));
                        if (snprintf(path, PATH_MAX, "%s", worm->d_name)
                                < PATH_MAX) {
                            catalogs[arr_size++] = path;
                            if (arr_size >= catalog_size) {
                                catalog_size *= 2;
                                catalogs = realloc(catalogs, catalog_size * sizeof(char *));   
                            }
                        }
                    }     
                }
            }      
        }
    }
    
    closedir(directory);

    qsort(catalogs, arr_size, sizeof(char *), comp);
    
    for (int i = 0; i < arr_size; ++i) {
        char full_path[PATH_MAX] = { 0 };
        if (snprintf(full_path, sizeof(full_path), "%s/%s", file, catalogs[i])
                < sizeof(full_path)) {
            recursive_traversal(full_path, false);
        }
    }
    
    if (!flag) {
        printf("cd ..\n");
    }
    
    for (int i = 0; i < arr_size; ++i) {
        free(catalogs[i]);
    }   
    
    free(catalogs);
    
    return;
}

int 
main(int argc, char *argv[])
{
    if (argc < REQUESTED_AMOUNT_OF_ARGS) return 1;
    
    const char *file = argv[FIRST_ARG];
    
    recursive_traversal(file, true);

    return 0;
}