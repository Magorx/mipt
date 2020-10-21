/*
На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка [Li,Ri]. 
Найти сумму длин частей числовой прямой, окрашенных ровно в один слой. N≤10000. Li,Ri — целые числа в диапазоне [0,109].
*/


#include <stdio.h>
#include <stdlib.h>

const int IN = 1;
const int OUT = -1;

typedef struct Vault_t {
	long long x;
	long long type;
} Vault;

int Vault_comparator(const Vault **first_ptr, const Vault **second_ptr) {
	const Vault *first = *first_ptr;
	const Vault *second = *second_ptr;

	return first->x - second->x;
}

// Получив отсортированные точки с их типом (открывают или закрывают отрезок), сможем, итерируясь по ним,
// понять, какова длина, закрашенная необходимым кол-вом отрезков одновременно

long long scanline_balace_x(const Vault **arr, const size_t elem_count, const int balance_x) {
	int balance = 0;
	long long ret = 0;
	for (size_t i = 0; i < elem_count; ++i) {
		if (balance == balance_x) {
			ret += arr[i]->x - arr[i - 1]->x;
		}
		balance += arr[i]->type;
	}
	return ret;
}

int main() {
	int n = 0;
	scanf("%d", &n);
	Vault **arr = calloc(n * 2, sizeof(Vault*));

	for (int i = 0; i < 2 * n; i += 2) {
		arr[i] = calloc(1, sizeof(Vault));
		arr[i + 1] = calloc(1, sizeof(Vault));
		long long x1, x2;
		scanf("%lld %lld", &x1, &x2);
		arr[i]->x = x1;
		arr[i]->type = IN;
		arr[i + 1]->x = x2;
		arr[i + 1]->type = OUT;
	}

	qsort(arr, 2 * n, sizeof(Vault*), Vault_comparator);

	/*for (int i = 0; i < 2 * n; ++i) {
		printf("%d) %lld %lld\n", i, arr[i]->x, arr[i]->type);
	}*/

	printf("%lld\n", scanline_balace_x((const Vault**)arr, 2 * n, 1));

	for (int i = 0; i < 2 * n; ++i) {
		free(arr[i]);
	}
	free(arr);

	return 0;
}

// O(nlogn) - сортировка и сканлайн за линию