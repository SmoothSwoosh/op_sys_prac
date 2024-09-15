#include <limits.h>

STYPE 
bit_reverse(STYPE value) 
{
    UTYPE ans = 0;
    value = (UTYPE)value;
    for (int i = 0; i < sizeof(UTYPE) * CHAR_BIT; ++i) {
        ans <<= 1;
        ans |= (value & 1);
        value >>= 1;
    }    
    
    return (STYPE)ans;
}