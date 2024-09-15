#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

enum
{
    ACCESS_BITS = 0x01FF,
    OTHER_SHIFT = 0,
    GROUP_SHIFT = 3,
    USER_SHIFT = 6,
    BIT_MASK = 0x7
};

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

static int
user_in_owners_group(
        const struct stat *stb,
        const struct Task *task)
{
    for (size_t i = 0; i < task->gid_count; ++i) {
        if (stb->st_gid == task->gids[i]) return 1;
    }
    
    return 0;
}

static int
check_access(
        unsigned int permission_bits, 
        unsigned int shift,
        unsigned int access)
{
    unsigned int checked_bits = (permission_bits >> shift) & BIT_MASK;
    
    return ((checked_bits & access) == access);
}

int
myaccess(
        const struct stat *stb, 
        const struct Task *task, 
        int access)
{
    if (stb == NULL || task == NULL) {
        return 0;
    }
    
    //to "simplify"
    if (task->uid == 0) {
        return 1;
    }
    
    unsigned int u_access = (unsigned int)access;
    
    unsigned int permission_bits = stb->st_mode & ACCESS_BITS; 
    
    unsigned int shift = 0;
    //user
    if (stb->st_uid == task->uid) {
        shift = USER_SHIFT;
    } else if (user_in_owners_group(stb, task)) {
        shift = GROUP_SHIFT;
    } else {
        shift = OTHER_SHIFT;
    }       
    return check_access(permission_bits, shift, u_access);
}