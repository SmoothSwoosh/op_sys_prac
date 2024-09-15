#include <stdio.h>
#include <stdlib.h>

const int len = 109;

int main(int argc, char **argv)
{
    int t = 0, v = 0, ans = 0;
    scanf("%d%d", &v, &t);
    ans = (v >= 0) ? v * t % len : (len - (-v) * t % len) % len;
    printf("%d ", ans);

    return 0;
}