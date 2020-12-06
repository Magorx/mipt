/*
Даны n отрезков на прямой. Пара отрезков называются вложенной, если отрезки не совпадают, и один отрезок содержит второй. Посчитать количество пар вложенных отрезков.
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

#ifndef GENERAL_VECTOR
#define GENERAL_VECTOR

#include <stdexcept>

template <typename T>
class Vector {
private:
	T *buffer;
	size_t cur_size;
	size_t capacity;

	void realloc_bufer(const size_t new_capacity) {
		capacity = new_capacity;
		T* ptr = (T*) realloc(buffer, capacity * sizeof(T));
		if (!ptr) {
			throw std::length_error("[ERR]<vector>: realloc fail");
		}
		buffer= ptr;
	}

public:
	Vector           (const Vector& other) = delete;
	Vector& operator=(const Vector& other) = delete;

	Vector() :
	buffer(nullptr),
	cur_size(0),
	capacity(0)
	{}

	void ctor() {
		capacity = 32;
		buffer = (T*) malloc(capacity * sizeof(T));
		cur_size = 0;
	}

	static Vector<T> *NEW() {
		Vector<T> *cake = (Vector<T>*) calloc(1, sizeof(Vector<T>));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	Vector(const size_t size_) :
	buffer(nullptr),
	cur_size(0),
	capacity(0)
	{}

	void ctor(const size_t size_) {
		capacity = cur_size * 2;
		buffer = (T*) calloc(capacity, sizeof(T));
		cur_size = size_;
	}

	static Vector<T> *NEW(const size_t size_) {
		Vector<T> *cake = (Vector<T>*) calloc(1, sizeof(Vector<T>));
		if (!cake) {
			return nullptr;
		}

		cake->ctor(size_);
		return cake;
	}

	~Vector() {}

	void dtor() {
		cur_size = 0;
		free(buffer);
	}

	static void DELETE(Vector<T> *vector) {
		if (!vector) {
			return;
		}

		vector->dtor();
		free(vector);
	}

	T &operator[](const size_t i) const {
		if (i >= cur_size) {
			throw std::length_error("[ERR]<vector>: index overflow");
		}
		return buffer[i];
	}

	T &push_back(const T &val) {
		if (cur_size + 1 == capacity) {
			realloc_bufer(capacity * 2);
		}
		return buffer[cur_size++] = val;
	}

	T &pop_back() {
		if (cur_size == 0) {
			throw std::length_error("[ERR]<vector>: pop underflow");
		}
		return buffer[--cur_size];
	}

	size_t size() const {
		return cur_size;
	}

	void quadratic_pointer_sort() {
		bool flag = false;
		for (size_t i = 0; i < size(); ++i) {
			for (size_t j = 0; j < size(); ++j) {
				if (*buffer[i] > *buffer[j]) {
					T tmp = buffer[i];
					buffer[i] = buffer[j];
					buffer[j] = tmp;
					flag = true;
				}
			}
			if (!flag) {
				return;
			}
		}
	}

	bool contains(const T &item) const {
		for (size_t i = 0; i < size(); ++i) {
			if (buffer[i] == item) {
				return true;
			}
		}
		return false;
	}

	void print_as_ints() const {
		if (!size()) {
			return;
		}

		for (size_t i = 0; i < size() - 1; ++i) {
			printf("%d ", (*this)[i]);
		}
		printf("%d", (*this)[size() - 1]);
	}
};

#endif // GENERAL_VECTOR

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

struct Segment {
	long long x;
	long long y;
	long long cnt;

	Segment() {
		x = 0;
		y = 0;
		cnt = 0;
	}

	Segment(long long x_, long long y_) {
		x = x_;
		y = y_;
		cnt = 1;
	}

	bool operator<(const Segment &other) {
		if (x < other.x) {
			return true;
		} else if (x > other.x) {
			return false;
		}

		if (cnt * y < cnt * other.y) {
			return false;
		} else if (cnt * y > cnt * other.y) {
			return true;
		}

		return false;
	}

	void swap_sides() {
		long long tmp = y;
		y = x;
		x = tmp;
		//cnt *= -1;
	}
};

template <typename T>
void merge(T *arr, const int left, const int middle, const int right, T *buffer) {
    int i = 0;
    int j = 0;
    while (i < middle - left && j < right - middle) {
        if (arr[left + i] < arr[middle + j]) {
            buffer[left + i + j] = arr[left + i];
            ++i;
        } else {
            buffer[left + i + j] = arr[middle + j];
            ++j;
        }
    }

    while(i < middle - left) {
        buffer[left + i + j] = arr[left + i];
        ++i;
    }

    while (j < right - middle) {
        buffer[left + i + j] = arr[middle + j];
        ++j;
    }

    for (int i = left; i < right; ++i) {
        arr[i] = buffer[i];
    }
}

template <typename T>
void do_merge_sort(T *arr, const int left, const int right, T *buffer) {
    if (right - left <= 1) { return; }

    int middle = (left + right) / 2;
    do_merge_sort(arr, left, middle, buffer);
    do_merge_sort(arr, middle, right, buffer);
    merge(arr, left, middle, right, buffer);
}

template <typename T>
void merge_sort(T *arr, const size_t arr_size) {
    T *buffer = (T*) malloc(arr_size * sizeof(T));

    do_merge_sort(arr, 0, arr_size, buffer);

    free(buffer);
}

// Отсортируем отрезки по левому концу, а правые их концы заменим на 1..n, чтобы через фенвика
// считать кол-во отрезков с ПК (правыми концами) левее ПК текущего отрезка. Это даст нам возможность
// считать кол-во отрезков вложенных в данный, т.к. из итерироваться мы будем слева направо, значит
// чтобы отрезок был вложен наш, хватит того, что его ПК <= нашему

int main() {
	int n = 0; 
	scanf("%d", &n);
	Segment *arr = (Segment*) calloc(n, sizeof(Segment));
	for (int i = 0; i < n; ++i) {
		long long x, y;
		scanf("%lld %lld", &x, &y);
		arr[i] = Segment(y, x);
	}

	merge_sort(arr, n);

	// printf("-----\n");
	// for (int i = 0; i < n; ++i) {
	// 	printf("%lld %lld\n", arr[i].x, arr[i].y);
	// }

	long long last  = arr[0].x;
	long long max_r = 0;
	for (int i = 0; i < n; ++i) {
		Segment &sg = arr[i];
		if (sg.x != last) {
			++max_r;
		}
		last = sg.x;
		sg.x = max_r;
		sg.swap_sides();
	}

	merge_sort(arr, n);

	Vector<Segment> a; // awful way not to think about segment duplicats
	a.ctor();

	a.push_back(arr[0]);
	Segment *pr = &a[0];
	for (int i = 1; i < n; ++i) {
		if (pr->x == arr[i].x && pr->y == arr[i].y) {
			pr->cnt += 1;
		} else {
			a.push_back(arr[i]);
			pr = &a[a.size() - 1];
		}
	}

	FenvikTree<long long, functor_sum, functor_sub> tree;
	tree.ctor(nullptr, max_r + 10, 0);

	FenvikTree<long long, functor_sum, functor_sub> tree_l;
	tree_l.ctor(nullptr, max_r + 10, 0);

	for (int i = 0; i < a.size(); ++i) {
		Segment &sg = a[i];
		tree.upd(sg.y, sg.cnt);
		tree_l.upd(sg.x, sg.cnt);
	}

	long long cnt = 0;
	for (int i = 0; i < a.size(); ++i) {
		Segment &sg = a[i];
		//printf("[%lld %lld] - %lld\n", sg.x, sg.y, sg.cnt);
		tree.upd(sg.y, -sg.cnt);
		tree_l.upd(sg.x, -sg.cnt);
		cnt += tree.get(0, sg.y) * sg.cnt;
		//cnt -= min(tree_l.get(sg.x, sg.x), tree.get(sg.y, sg.y));
	}

	printf("%lld\n", cnt);

	return 0;
}

// O(O)