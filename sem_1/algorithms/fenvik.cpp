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

		if (arr) {
			for (int i = 0; i < size; ++i) {
				set(i, arr[i]);
			}
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

int main() {
	FenvikTree<long long, functor_sum, functor_sub> tree;
	tree.ctor(nullptr, 10, 0);

	return 0;
}