#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
cmp(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

int main(int argc, char **argv)
{
    int n = 0;
    scanf("%d", &n);
    int *arr = calloc(101, sizeof(int));
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }
    qsort(arr, n, sizeof(int), cmp);
    for (int i = 0; i < n; ++i) {
        if (arr[i] % 2 == 0) printf("%d ", arr[i]);
    }
    for (int i = 0; i < n; ++i) {
        if (arr[i] % 2 == 1) printf("%d ", arr[i]);
    }

    return 0;
}