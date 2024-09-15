#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

enum
{
    FIRST_ARG = 1,
    SECOND_ARG = 2,
    THIRD_ARG = 3,
    FUNC_ARGS = 4,
    BASE = 10,
    OVERALL_SIZE = 64
};

typedef struct FuncArguments
{
    char args[OVERALL_SIZE];
} FuncArguments;

int 
main(int argc, char *argv[])
{
    char *ptr_lib = dlopen(argv[FIRST_ARG], RTLD_LAZY);
    
    if (ptr_lib == NULL) {
        return 1;
    }
   
    FuncArguments st = { };
    memset(&st, 0, sizeof(st));
    
    int index = 0;
    
    for (int i = FUNC_ARGS, j = 1; i < argc; ++i, ++j) {
        if (argv[THIRD_ARG][j] == 'i') {
            int number = strtol(argv[i], NULL, BASE);  
            memcpy(&st.args[index], &number, sizeof(number));
            index += sizeof(number);
        } else if (argv[THIRD_ARG][j] == 'd') {
            double number = strtod(argv[i], NULL);
            memcpy(&st.args[index], &number, sizeof(number));
            index += sizeof(number);
        } else {
            memcpy(&st.args[index], &argv[i], sizeof(argv[i]));
            index += sizeof(argv[i]);
        }
    }
    
    void *func_address = dlsym(ptr_lib, argv[SECOND_ARG]);
    
    if (func_address == NULL) {
        dlclose(ptr_lib);
        return 1;
    }

    if (argv[THIRD_ARG][0] == 'v') {
        ((void (*)(FuncArguments))func_address)(st);
    } else if (argv[THIRD_ARG][0] == 'i') {
        printf("%d\n", 
                ((int (*)(FuncArguments))func_address)(st));
    } else if (argv[THIRD_ARG][0] == 'd') {
        printf("%.10g\n", 
                ((double (*)(FuncArguments))func_address)(st));
    } else {
        printf("%s\n", 
                ((char *(*)(FuncArguments))func_address)(st));
    }

    if (dlclose(ptr_lib) != 0) {
        return 1;
    }

    return 0;
}
