int
satsum(int v1, int v2) 
{
    int int_max = (unsigned)(~0) >> 1;
    int int_min = ~int_max;
    if (v1 >= 0 && v2 >= 0) {
        if (int_max - v1 < v2) {
            return int_max;
        }
    } else if (v1 <= 0 && v2 <= 0) {
        if (int_min - v1 > v2) {
            return int_min;
        }
    } 
    
    return v1 + v2;
}