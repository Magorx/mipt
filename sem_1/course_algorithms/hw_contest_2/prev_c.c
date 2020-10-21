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
 
long long partition(long long *arr, int elem_cnt, int k) {
	if (arr[0] == arr[elem_cnt - 1]) {
		return arr[0];
	}
 
	int pivot_idx = randlong() % elem_cnt;
	long long pivot = arr[pivot_idx];
 
	long long *l = &arr[0];
	long long *r = &arr[elem_cnt - 1];
	while (1) {
		//printf("%ld, %ld\n", l - arr, r - arr);
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
 
	int idx = (int) (l - arr);
	printf("parted to %d by %lld\n", idx, pivot);
	print_arr(arr, elem_cnt);
 
	if (idx > k) {
		return partition(arr, idx, k);
	} else {
		return partition(l, elem_cnt - idx, k - idx);
	}
 
	return 0;
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
 
	printf("%lld\n", partition(arr, n, k));
 
	free(arr);
 
	return 0;
}
 