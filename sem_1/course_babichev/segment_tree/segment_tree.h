
template <typename T>
struct SegmentTreeNode {
	T value;
	T xor_res;
	T max_res;

	SegmentTreeNode () {}
	~SegmentTreeNode() {}
 
	ctor (T val) {
		value = val;
		xor_res = value;
		max_res = value;
	}

	SegmentTreeNode operation(const T &other) {
		SegmentTreeNode res;
		res.value   = value   + other.value;
		res.xor_res = xor_res + other.xor_res;
		res.max_res = max_res + other.max_res;
		return res;
	}
}


template <typename T, typename T_OP>
class SegmentTree {
private:
	T *buffer;
	size_t size;
	T neutral;

public:
	SegmentTree () {}
	~SegmentTree() {}

	void ctor(const T *arr, const size_t len, const T neutral_) {
		size = 1;
		while (size <= len) {
			size *= 2;
		}

		buffer = (T*) calloc(size, sizeof(T));
		neutral = neutral_;
	}

	void build(const T *arr, int v, int vl, int vr) {
		if (vl == vr - 1) {
			buffer[v] = arr[vl];
		} else {
			int vm = (vl + vr) / 2;
			build(arr, 2 * v + 1, vl, vm);
			build(arr, 2 * v + 2, vm, vr);
			buffer[v] = T_OP()(buffer[2 * v + 1], buffer[2 * v + 2]]);
		}
	}

	T find(int v, int vl, int vm, int l, int r) {
		if (l > r || vr <= l || vl >= r) {
			return neutral;
		}
		if (l <= vl && vr <= r) {
			return buffer[v];
		}

		int vm = (vl + vr) / 2;
		return T_OP()(find(2 * v + 1, vl, vm, l, r), find(2 * v + 2, vm, vr, l, r));
	}

	void dtor() {
		free(buffer);
	}
}

int 