#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const long long BASE = 256;

int number_len(long long n) {
	int ret = 0;
	while (n) {
		++ret;
		n /= BASE;
	}
	return ret;
}

int num_digit(long long n, int idx) {
	long long div = pow(BASE, idx);
	return (n / div) % BASE;
}

void swap_ptrs(long long **a, long long **b) {
	long long *tmp = *b;
	*b = *a;
	*a = tmp;
}

int radix_sort(long long *arr, int elem_cnt) {
	int max_len = 0; // считаем максимальную длину числа
	for (int i = 0; i < elem_cnt; ++i) {
		int n_len = number_len(arr[i]);
		max_len = max_len > n_len ? max_len : n_len;
	}
	max_len += 1;

	long long base = BASE;
	long long *d_cnt = (long long *)  calloc(base + 1, sizeof(long long)); // массив для сортировки подсчетом чисел по цифрам
	long long *buffer = (long long *) calloc(elem_cnt, sizeof(long long)); // доп. буффер для перемещение эл-тов

	for (int d_idx = 0; d_idx <= max_len; ++d_idx) {
		for (int i = 0; i < base; ++i) {
			d_cnt[i] = 0;
		}
		for (int i = 0; i < elem_cnt; ++i) {
			int d = num_digit(arr[i], d_idx);
			++d_cnt[d];
		}

		int pos_cnt = 0;
		// из d_cnt делаем массив префиксных сумм
		for (int d = 0; d < base; ++d) {
			int d_counted = d_cnt[d];
			d_cnt[d] = pos_cnt;
			pos_cnt += d_counted;
		}

		// расставляем числа по нужным местам в buffer
		for (int i = 0; i < elem_cnt; ++i) {
			int d = num_digit(arr[i], d_idx);
			buffer[d_cnt[d]] = arr[i];
			++d_cnt[d];
		}

		// возвращаем в наш массив
		for (int i = 0; i < elem_cnt; ++i) {
			arr[i] = buffer[i];
		}
	}

	free(d_cnt);
	free(buffer);
	return 0;
}

int main() {
	int n = 0;
	scanf("%d", &n);
	long long *arr = (long long *) calloc(n, sizeof(long long));
	for (int i = 0; i < n; ++i) {
		scanf("%lld", &arr[i]);
	}

	radix_sort(arr, n);
	for (int i = 0; i < n; ++i) {
		printf("%lld ", arr[i]);
	}
	printf("\n");

	free(arr);

	return 0;
}

// O(n + |d|), |d| - мощность алфавита

/* 
	я даже стрессил решение, он же говорил wa5, почему на плюсов зашло.............

	for (int z = 0; z < 100; ++z) {
		printf("i\n");
		for (int i = 0; i < n; ++i) {
			arr[i] = rand();
		}
		radix_sort(arr, n);
		for (int i = 0; i < n - 1; ++i) {
			if (arr[i] > arr[i + 1]) {
				printf("fuck %lld %lld\n", arr[i], arr[i + 1]);
				return 0;
			}
		}
	}
	*/
