/*
Вася любит наблюдать за звездами. Но следить за всем небом сразу ему тяжело. Поэтому он
наблюдает только за частью пространства, ограниченной кубом размером n×n×n. Этот куб поделен
на маленькие кубики размером 1 × 1 × 1. Во время его наблюдений могут происходить следующие
события:
1. В каком-то кубике появляются или исчезают несколько звезд.
2. К нему может заглянуть его друг Петя и поинтересоваться, сколько видно звезд в части пространства, состоящей из нескольких кубиков.
*/

#include <cstdlib>
#include <cstdio>
#include <climits>

template <typename T>
const T &min(const T &first, const T &second) {
	return first < second ? first : second;
}

template <typename T>
const T &max(const T &first, const T &second) {
	return second < first ? first : second;
}

template <typename T, typename T_OP, typename T_REV>
class FenvikTree3d {
private:
	T ***buffer;
	size_t size;
	T neutral;

	const T get(int i_, int j_, int k_) {
		T ret = neutral;
		for (int i = i_; i >= 0; i = (i & (i + 1)) - 1)
			for (int j = j_; j >= 0; j = (j & (j + 1)) - 1)
				for (int k = k_; k >= 0; k = (k & (k + 1)) - 1)
					ret = T_OP()(ret, buffer[i][j][k]);
		return ret;
	}

public:
	FenvikTree3d () {}
	~FenvikTree3d() {}

	void ctor(const T *arr, const size_t arr_size, const  T &neutral_) {
		buffer = (T***) calloc(arr_size + 1, sizeof(T**));
		for (int i = 0; i < arr_size + 1; ++i) {
			buffer[i] = (T**) calloc(arr_size + 1, sizeof(T*));
			for (int j = 0; j < arr_size + 1; ++j) {
				buffer[i][j] = (T*) calloc(arr_size + 1, sizeof(T));
			}
		}

		if (arr) {
			for (int i = 0; i < arr_size; ++i) {
				for (int j = 0; j < arr_size; ++j) {
					for (int k = 0; k < arr_size; ++k) {
						upd(i, j, k, arr[i, j, k]);
					}
				}
			}
		}

		size = arr_size;
		neutral = neutral_;
	}

	void dtor() {
		for (int i = 0; i < size + 1; ++i) {
			for (int j = 0; j < size + 1; ++j) {
				free(buffer[i][j]);
			}
			free(buffer[i]);
		}
		free(buffer);
	}

	void upd(int i_, int j_, int k_, const T &val) {
		for (int i = i_; i < size; i |= i + 1)
			for (int j = j_; j < size; j |= j + 1)
				for (int k = k_; k < size; k |= k + 1)
					buffer[i][j][k] = T_OP()(buffer[i][j][k], val);
	}

	T get(int x1, int y1, int z1, int x2, int y2, int z2) {
		--x1;
		--y1;
		--z1;
		long long ret   = get(x2, y2, z2);
		
		long long sub_1 = get(x2, y2, z1);
		long long sub_2 = get(x2, y1, z2);
		long long sub_3 = get(x1, y2, z2);
		
		long long add_4 = get(x1, y1, z2);
		long long add_5 = get(x1, y2, z1);
		long long add_6 = get(x2, y1, z1);

		long long sub_7 = get(x1, y1, z1);

		return ret - sub_1 - sub_2 - sub_3 + add_4 + add_5 + add_6 - sub_7;
	}
};

struct functor_sum {
public:
	long long operator()(long long a, long long b) {
		return a + b;
	}
};

struct functor_sub {
public:
	long long operator()(long long a, long long b) {
		return a - b;
	}
};

// Многомерный Фенвик :0

int main() {
	int n = 0; 
	scanf("%d", &n);
	
	FenvikTree3d<long long, functor_sum, functor_sub> tree;
	tree.ctor(nullptr, n + 2, 0);

	while (true) {
		int mode = 0;
		scanf("%d", &mode);

		if (mode == 1) {
			long long x, y, z, k;
			scanf("%lld %lld %lld %lld", &x, &y, &z, &k);
			tree.upd(x, y, z, k);
		} else if (mode == 2) {
			long long x1, y1, z1, x2, y2, z2;
			scanf("%lld %lld %lld %lld %lld %lld", &x1, &y1, &z1, &x2, &y2, &z2);
			printf("%lld\n", tree.get(x1, y1, z1, x2, y2, z2));
		} else {
			break;
		}
	}

	tree.dtor();

	return 0;
}

// O(O(O(O)))
// O((logn)^3)