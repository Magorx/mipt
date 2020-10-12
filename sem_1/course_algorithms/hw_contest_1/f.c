/*
    Напишите программу, которая для заданного массива A=⟨a1,a2,…,an⟩ находит количество пар (i,j) таких, что i<j и ai>aj. Обратите внимание на то, что ответ может не влезать в int.

    Входные данные
    Первая строка входного файла содержит натуральное число n (1⩽n⩽100000) — количество элементов массива. Вторая строка содержит n попарно различных элементов массива A — целых неотрицательных чисел, не превосходящих 109.

    Выходные данные
    В выходной файл выведите одно число — ответ на задачу.
*/

#include <stdio.h>
#include <stdlib.h>

long long merge_sort(int *arr, const size_t arr_size);
void do_merge_sort(int *arr, const int left, const int right, int *buffer, long long *inv_cnt);
void merge(int *arr, const int left, const int middle, const int right, int *buffer, long long *inv_cnt);

int main() {
    FILE *fin = fopen("inverse.in", "r");

    int n = 0;
    fscanf(fin, "%d", &n);

    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        fscanf(fin, "%d", &arr[i]);
    }
    close(fin);

    long long rev_cnt = merge_sort(arr, n);

    FILE *fout = fopen("inverse.out", "w");
    fprintf(fout, "%lld\n", rev_cnt);
    close(fout);

    free(arr);

    return 0;
}

void merge(int *arr, const int left, const int middle, const int right, int *buffer, long long *inv_cnt) {
    int i = 0;
    int j = 0;
    while (i < middle - left && j < right - middle) {
        if (arr[left + i] < arr[middle + j]) {
            buffer[left + i + j] = arr[left + i];
            ++i;
            *inv_cnt += j;
        } else {
            buffer[left + i + j] = arr[middle + j];
            ++j;
        }
    }

    while(i < middle - left) {
        buffer[left + i + j] = arr[left + i];
        ++i;
        *inv_cnt += j;
    }

    while (j < right - middle) {
        buffer[left + i + j] = arr[middle + j];
        ++j;
    }

    for (int i = left; i < right; ++i) {
        arr[i] = buffer[i];
    }
}

void do_merge_sort(int *arr, const int left, const int right, int *buffer, long long *inv_cnt) {
    if (right - left <= 1) { return; }

    int middle = (left + right) / 2;
    do_merge_sort(arr, left, middle, buffer, inv_cnt);
    do_merge_sort(arr, middle, right, buffer, inv_cnt);
    merge(arr, left, middle, right, buffer, inv_cnt);
}

long long merge_sort(int *arr, const size_t arr_size) {
    int *buffer = malloc(arr_size * sizeof(int));
    long long ret = 0;

    do_merge_sort(arr, 0, arr_size, buffer, &ret);

    free(buffer);
    return ret;
}

// O(nlogn) - mergesort
