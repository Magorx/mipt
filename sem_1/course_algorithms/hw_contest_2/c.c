/*
Даны неотрицательные целые числа N, K и массив целых чисел из диапазона [0,109] размера N. Требуется найти K-ю порядковую статистику, т.е. напечатать число, которое бы стояло на позиции с индексом K ∈[0,N−1] в отсортированном массиве.

Напишите нерекурсивный алгоритм. Требования к дополнительной памяти: O(1). Требуемое среднее время работы: O(N).
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long randlong() {
	long ret = rand();
	ret |= rand() << sizeof(int);
	return abs(ret);
}

void swap_ll(long long *first, long long *second) {
	long long tmp = *first;
	*first = *second;
	*second = tmp;
}

void print_arr(long long *arr, long long elem_cnt) {
	for (int i = 0; i < elem_cnt; ++i) {
		printf("[%d] %lld\n", i, arr[i]);
	}
}


// Заданный массив по случайному его элементу разбивает на отрезки "меньше" и "больше или равно"
int partition(long long *arr, int elem_cnt) {
	int pivot_idx = randlong() % elem_cnt;
	long long pivot = arr[pivot_idx];

	long long *l = &arr[0];
	long long *r = &arr[elem_cnt - 1];
	while (1) {
		while (*l <  pivot && l < r) {
			++l;
		}
		while (*r >= pivot && l < r) {
			--r;
		}

		swap_ll(l, r);

		if (l >= r) {
			break;
		}
	}

	return (int) (l - arr);
}


// Используя partition и зная, сколько элементов точно меньше того, по которому он решил разбить массив,
// можно понять, с какой стороны от границы находится искомая статистика k
long long k_ordered_stat(long long *arr, int elem_cnt, int k) {
	while (elem_cnt) {
		if (arr[0] == arr[elem_cnt - 1]) {
			return arr[0];
		}

		int idx = partition(arr, elem_cnt);

		if (idx > k) {
			elem_cnt = idx;
		} else {
			arr = &arr[idx];
			elem_cnt -= idx;
			k -= idx;
		}
	}

	return *arr;
}

int main() {
	srand(time(NULL));
	int n = 0;
	int k = 0;
	scanf("%d %d", &n, &k);

	long long *arr = calloc(n, sizeof(long long));
	for (int i = 0; i < n; ++i) {
		scanf("%lld", &arr[i]);
	}

	printf("%lld\n", k_ordered_stat(arr, n, k));

	free(arr);

	return 0;
}

// O(n) - так в задаче требуется