#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
max(int a, int b)
{
    return (a > b) ? a : b;
}

double ***
transpose(double ***arr)
{
    int arr_size = 0;
    double **ptr = arr[arr_size];
    
    int max_len = 0;
    
    while (ptr != NULL) {
        ptr = arr[++arr_size];
        int row_size = 0;
        double *str_ptr = arr[arr_size - 1][0];
        while (str_ptr != NULL) {
            str_ptr = arr[arr_size - 1][++row_size];
        }
        
        max_len = (row_size > max_len) ? row_size : max_len;
    }
    
    int *next_max_entries = (int *)calloc(arr_size, sizeof(int));
    int *row_lens = (int *)calloc(arr_size, sizeof(int));
    int *column_lens = (int *)calloc(max_len, sizeof(int));
    
    for (int i = arr_size - 1; i >= 0; --i) {
        int row_size = 0;
        double *str_ptr = arr[i][0];
        
        while (str_ptr != NULL) {
            column_lens[row_size]++;
            str_ptr = arr[i][++row_size];
        }
        
        row_lens[i] = row_size;
        
        if (i != 0) {
            next_max_entries[i - 1] = max(row_size, next_max_entries[i]);
        }
    }
    
    for (int i = 0; i < arr_size; ++i) {
        for (int j = row_lens[i]; j < max_len; ++j) {
            if (j < next_max_entries[i]) {
                column_lens[j]++;
            }
        }
    }
    
    double ***answer = (double ***)calloc(max_len + 1, sizeof(double **));
    answer[max_len] = NULL;
    
    for (int i = 0; i < max_len; ++i) {
        answer[i] = (double **)calloc(column_lens[i] + 1, sizeof(double *));
    }
    
    for (int i = 0; i < max_len; ++i) {
        for (int j = 0; j < column_lens[i]; ++j) {
            answer[i][j] = (double *)calloc(1, sizeof(double));
            *answer[i][j] = (i < next_max_entries[j] && i >= row_lens[j]) ? 0 : *arr[j][i];
        }
    }
    
    for (int i = 0; i < max_len; ++i) {
        answer[i][column_lens[i]] = NULL;
    }

    free(next_max_entries);
    free(row_lens);
    free(column_lens);
    
    return answer;
}