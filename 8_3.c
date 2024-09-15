#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

enum
{
    FIRST_ARG = 1,
    SECOND_ARG = 2,
    THIRD_ARG = 3,
    BASE = 10
};

void
fill_matrix(int *arr, int rows, int cols)
{
    int right_border = cols;
    int down_border = rows;
    int left_border = 0;
    int up_border = 1;
    int counter = 1;
    int last_number = rows * cols;
    
    int i = 0, j = 0; 
    
    while (counter <= last_number) {
        //right move
        for (; (j < right_border) && (counter <= last_number); ++j, counter++) {
            arr[i * cols + j] = counter;
        }
        right_border--, j--, i++;
        
        //down move
        for (; (i < down_border) && (counter <= last_number); ++i, counter++) {
            arr[i * cols + j] = counter;
        }
        down_border--, i--, j--;

        //left move
        for(; (j >= left_border) && (counter <= last_number); --j, counter++) {
            arr[i * cols + j] = counter;
        }
        left_border++, j++, i--;
        
        //up move
        for(; (i >= up_border) && (counter <= last_number); --i, counter++) {
            arr[i * cols + j] = counter;
        }
        up_border++, i++, j++;
    }
}

int 
main(int argc, char *argv[])
{
    int fd = open(argv[FIRST_ARG], O_RDWR | O_TRUNC | O_CREAT, 0600);
    int rows = strtol(argv[SECOND_ARG], NULL, BASE);
    int cols = strtol(argv[THIRD_ARG], NULL, BASE);
    
    int size = 0, size_rows = 0;
    if (__builtin_smul_overflow(sizeof(int), rows, &size_rows) != 0) {
        return 1;
    }
    
    if (__builtin_smul_overflow(size_rows, cols, &size) != 0) {
        return 1;
    }
    
    if (fd < 0) {
        return 1;
    }
    
    if (ftruncate(fd, size) == -1) {
        close(fd);
        return 1;
    }
    
    int *ptr = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (ptr == MAP_FAILED) {
        close(fd);
        return 1;
    }
    
    fill_matrix(ptr, rows, cols);

    if (munmap(ptr, size) != 0) {
        close(fd);
        return 1;
    }
    
    close(fd);

    return 0;
}