#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    unsigned int number = 0;
    unsigned char bytes[4];
    while (scanf("%u", &number) == 1) {
        bytes[3] = number & 0xFF;
        bytes[2] = (number >> 8) & 0x0F;
        bytes[1] = (number >> 12) & 0xFF;
        bytes[0] = (number >> 20) & 0x0F;
        int size_write = 0;
        while (size_write < sizeof(bytes)) {
            size_write += write(fd, bytes, sizeof(bytes));
        }
    }
    
    close(fd);

    return 0;
}