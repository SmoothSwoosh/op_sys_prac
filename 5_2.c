#include <stdio.h>
#include <fcntl.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


enum { RADIX = 8 };

int 
main(int argc, char *argv[])
{
    const char template[] = "rwxrwxrwx";
    const int template_size = sizeof(template) - 1;
    char answer[sizeof(template)];
    for (int i = 1; i < argc; ++i) {
        memset(answer, 0, sizeof(answer));
        unsigned long number = strtol(argv[i], NULL, RADIX);
        int index = template_size - 1;
        while (index >= 0) {
            if (!(number & 1)) {
                answer[index] = '-';
            } else {
                answer[index] = template[index];
            }
            --index;
            number >>= 1;
        }
        answer[template_size] = '\0';
        printf("%s\n", answer);
    }
    
    return 0;
}
