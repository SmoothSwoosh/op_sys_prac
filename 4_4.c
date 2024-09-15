#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <stdint.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void 
output(int fd, long long index) 
{
    struct Node worm = {};
    if (lseek(fd, index * sizeof(worm), SEEK_SET) == -1) {
        return;
    }
    int cnt_read = read(fd, &worm, sizeof(worm));
    if (cnt_read != sizeof(worm)) {
        return;
    }
    
    if (worm.right_idx != 0) {
        output(fd, worm.right_idx);
    }
    
    printf("%d\n", worm.key);
    
    if (worm.left_idx != 0) {
        output(fd, worm.left_idx);
    }    
    
    return;
}

int 
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    
    if (fd == -1) {
        return 0;
    }
    
    output(fd, 0);

    return 0;
}