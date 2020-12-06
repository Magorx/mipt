/*
Реализуйте структуру данных из n элементов a1,a2…an, поддерживающую следующие операции:

присвоить элементу ai значение j;
найти знакочередующуюся сумму на отрезке от l до r включительно (al−al+1+al+2−…±ar).
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
class FenvikTree {
private:
	T *buffer;
	size_t size;
	T neutral;

	const T get(int i) {
		T ret = neutral;
		for (; i >= 0; i = (i & (i + 1)) - 1) {
			ret = T_OP()(ret, buffer[i]);
		}
		return ret;
	}

public:
	FenvikTree () {}
	~FenvikTree() {}

	void ctor(const T *arr, const size_t arr_size, const  T &neutral_) {
		buffer = (T*) calloc(arr_size + 1, sizeof(T));
		size = arr_size;
		neutral = neutral_;

		for (int i = 0; i < size; ++i) {
			set(i, arr[i]);
		}
	}

	void dtor() {
		free(buffer);
	}

	const T &operator[](int i) {
		if (i >= size) {
			throw "Bad index";
		}

		return get(i, i);
	}

	void upd(int i, const T &val) {
		for (; i < size; i |= i + 1) {
			buffer[i] = T_OP()(buffer[i], val);
		}
	}

	void set(int i, const T &val) {
		T delta = T_REV()(val, get(i, i));
		upd(i, delta);
	}

	T get(int l, int r) {
		return T_REV()(get(r), get(l - 1));
	}

	void dump() {
		for (int i = 0; i < size; ++i) {
			printf("%lld ", buffer[i]);
		}
		printf("\n");
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


// Our lovely Fenvik ~~*

int main() {
	int n = 0; 
	scanf("%d", &n);
	long long *arr = (long long*) calloc(n, sizeof(long long));
	for (int i = 0; i < n; ++i) {
		scanf("%lld", &arr[i]);
		if (i % 2) {
			arr[i] *= -1;
		}
	}

	FenvikTree<long long, functor_sum, functor_sub> tree;
	tree.ctor(arr, n, 0);

	//tree.dump();

	int k = 0;
	scanf("%d", &k);
	for (int z = 0; z < k; ++z) {
		long long mode, l, r;
		scanf("%lld %lld %lld", &mode, &l, &r);

		if (mode == 0) {
			--l;
			if (l % 2) {
				r *= -1;
			}
			tree.set(l, r);
		} else {
			--l;
			--r;
			long long res = tree.get(l, r);
			if (l % 2) {
				res *= -1;
			}
			printf("%lld\n", res);
		}
	}

	return 0;
}

// Время исполнения в 19.5 раз быстрее максимального, судя по всему, O(klogn)