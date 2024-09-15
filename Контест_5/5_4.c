#include <stdio.h>
#include <fcntl.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

const char template[] = "rwxrwxrwx";

enum
{
    TEMPLATE_LENGTH = sizeof(template) - 1
};

int
parse_rwx_permissions(const char *str)
{
    //if pointer is NULL
    if (str == NULL) {
        return -1;
    }
    
    //is string a correct in terms of rwx order
    for (int i = 0; i < TEMPLATE_LENGTH; ++i) {
        if ((str[i] != template[i] && str[i] != '-')
                || (str[i] == '\0')) {
            return -1;        
        }
    }

    //if string is longer than template
    if (str[TEMPLATE_LENGTH] != '\0') {
        return -1;
    }
    
    unsigned short umask = 0; //answer
    for (int i = 0; i < TEMPLATE_LENGTH; ++i) {
        umask <<= 1;
        umask |= (str[i] != '-');
    }
    
    return umask;
}