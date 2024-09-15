#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <inttypes.h>

enum
{
    FIRST_COMMAND_ARG = 1,
    SECOND_COMMAND_ARG = 2,
    OFFSET_MASK = 0x1FF,
    OFFSET_BITS = 9,
    BASE = 16,
    FILE_SIZE = 1 << 16
};

int 
main(int argc, char *argv[])
{
    int fd = open(argv[FIRST_COMMAND_ARG], O_RDONLY);
    
    if (fd < 0) {
        return 1;
    }

    uint16_t pte_address = strtol(argv[SECOND_COMMAND_ARG], NULL, BASE);
    struct stat fs = { };
   
    if (fstat(fd, &fs) < 0) {
        close(fd);
        return 1;
    }
    
    if (fs.st_size != FILE_SIZE) {
        close(fd);
        return 1;
    }

    char *ptr = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        close(fd);
        return 1;
    }

    uint16_t virtual_address = 0;
    uint16_t *physical_address;
    while (scanf("%" SCNx16, &virtual_address) == 1) {
        uint16_t offset = virtual_address & OFFSET_MASK;
        unsigned int page_index = virtual_address >> OFFSET_BITS;
        uint16_t physical_page_index = *(uint16_t *)(ptr + pte_address
                + sizeof(uint16_t) * page_index);
        physical_address = (uint16_t *)
                (ptr + (physical_page_index | offset));
        printf("%d\n", *physical_address);
    }
    
    if (munmap(ptr, fs.st_size) != 0) {
        close(fd);
        return 1;
    }
    
    close(fd);
    
    return 0;
}