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

template <typename T, typename T_OP>
class SegmentTree {
private:
	T *buffer;
	T *differ;
	size_t buf_size;
	size_t size;
	size_t arr_pos;
	T neutral;

	T get_val(int v, int vl, int vr) {
		return buffer[v];
	}

	T find(int v, int vl, int vr, int l, int r) {
		if (l > r || vr <= l || vl >= r) {
			return neutral;
		}
		if (l <= vl && vr <= r) {
			return buffer[v];
		}

		int vm = (vl + vr) / 2;
		push(v);
		update(v, vl, vr);
		return T_OP()(find(2 * v + 1, vl, vm, l, r), find(2 * v + 2, vm, vr, l, r));
	}

	void seg_diff(int v, int vl, int vr, int l, int r, T diff) {
		if (l > r || vr <= l || vl >= r) {
			return;
		}
		if (l <= vl && vr <= r) {
			differ[v] = T_OP()(differ[v], diff);
			return;
		}

		int vm = (vl + vr) / 2;
		seg_diff(2 * v + 1, vl, vm, l, r, diff);
		seg_diff(2 * v + 2, vm, vr, l, r, diff);
		update(v, vl, vr);
	}

	void update(int v, int vl, int vr) {
		int vm = (vl + vr) / 2;
		buffer[v] = T_OP()(get_val(2 * v + 1, vl, vm), get_val(2 * v + 2, vm, vr));
	}

	void push(int v) {
		differ[2 * v + 1] = T_OP()(differ[2 * v + 1], differ[v]);
		differ[2 * v + 2] = T_OP()(differ[2 * v + 2], differ[v]);
		differ[v] = neutral;
	}

	void set(int v, int vl, int vr, int idx, const T &val) {
		if (vl == vr - 1) {
			buffer[v] = val;
		} else {
			int vm = (vl + vr) / 2;
			if (idx < vm) {
				set(2 * v + 1, vl, vm, idx, val);
			} else {
				set(2 * v + 2, vm, vr, idx, val);
			}
			update(v, vl, vr);
		}
	}

public:
	SegmentTree () {}
	~SegmentTree() {}

	void ctor(const T *arr, const size_t len, const T neutral_) {
		buf_size = 1;
		while (buf_size <= len) {
			buf_size *= 2;
		}
		buf_size *= 2;

		buffer = (T*) calloc(buf_size, sizeof(T));
		differ = (T*) calloc(buf_size, sizeof(T));

		size = len;
		neutral = neutral_;

		build(arr, 0, 0, len);
	}

	void build(const T *arr, int v, int vl, int vr) {
		if (vl == vr - 1) {
			if (vl == 0) {
				arr_pos = v;
			}
			buffer[v] = arr[vl];
		} else {
			int vm = (vl + vr) / 2;
			build(arr, 2 * v + 1, vl, vm);
			build(arr, 2 * v + 2, vm, vr);
			buffer[v] = T_OP()(buffer[2 * v + 1], buffer[2 * v + 2]);
		}
	}

	T find(int l, int r) {
		return find(0, 0, size, l, r);
	}

	void set(int idx, const T &val) {
		set(0, 0, size, idx, val);
	}

	void seg_diff(int l, int r, T diff) {
		seg_diff(0, 0, size, l, r, diff);
	}

	void dtor() {
		free(buffer);
		free(differ);
	}
};

struct Node {
	long long x;
	long long mn;
	long long mx;

	Node() {}

	Node(long long x_) {
		x  = x_;
		mn = x_;
		mx = x_;
	}

	Node(long long x_, long long mn_, long long mx_) {
		x  = x_;
		mn = mn_;
		mx = mx_;
	}
};

struct functor_sum {
public:
	Node operator()(const Node &a, const Node &b) {
		return Node(a.x, min(a.mn, b.mn), max(a.mx, b.mx));
	}
};

int main() {
	int n = 100009;
	Node *arr = (Node*) calloc(n, sizeof(Node));
	for (long long i = 0; i < n; ++i) {
		long long x = ((i*i) % 12345) + ((i*i*i) % 23456);
		arr[i] = Node(x);
	}

	SegmentTree<Node, functor_sum> tree;
	tree.ctor(arr, n, Node(-7777777, 99999999, -99999999));

	return 0;
}