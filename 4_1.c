#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

enum { SIZE = 6 };

int 
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    
    if (fd == -1) {
        return 0;
    }
    
    unsigned long long entries[SIZE] = {0};
    unsigned long long cur_entries[SIZE] = {0};
    unsigned long long index = 0, prev_no_ones = 0;
    unsigned short block_size = 0, prev_amount = 0, true_amount = 0;
    while (read(fd, &block_size, sizeof(unsigned short)) > 0) {
        read(fd, &true_amount, sizeof(unsigned short));
        if (index != 0) {
            if (true_amount != prev_amount) {
                unsigned long long twos = true_amount - prev_amount - prev_no_ones;
                cur_entries[0] -= twos;
                unsigned long long insert = twos;
                for (int i = 1; i < SIZE; ++i) {
                    //swap(insert, cur_entries[i]);
                    int tmp = insert;
                    insert = cur_entries[i];
                    cur_entries[i] = tmp;
                }
            }
            //entries += cur_entries and cur_entries = 0
            for (int i = 0; i < SIZE; ++i) {
                entries[i] += cur_entries[i];
                cur_entries[i] = 0;
            }
        }
        unsigned short entry = 0;
        prev_amount = 0;
        prev_no_ones = 0;
        for (int i = 0; i < block_size; ++i) {
            read(fd, &entry, sizeof(unsigned short));
            cur_entries[entry - 1]++;
            prev_amount += entry;
            if (entry != 1) {
                prev_no_ones++;
            }
        }
        ++index;
    }
    
    for (int i = 0; i < SIZE; ++i) {
        printf("%lld ", entries[i]);
    }
    
    printf("\n");
    close(fd);
    
    return 0;
}