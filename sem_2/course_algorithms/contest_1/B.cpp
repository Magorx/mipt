// Вам требуется написать программу, которая по заданной последовательности находит максимальную невозрастающую её подпоследовательность
// (т.е такую последовательность чисел ai1,ai2,…,aik (i1<i2<…<ik), что ai1≥ai2≥…≥aik и не существует последовательности с теми же свойствами длиной k+1).
 
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <algorithm>
#include <vector>
 
using namespace std;
 
template <typename T, typename T_OP>
class FenvikTree {
private:
	T *buffer;
	size_t size;
	T neutral;
 
public:
	FenvikTree () {}
	~FenvikTree() {}
 
	T *get_buffer() {
        return buffer;
	}
 
	void ctor(const T *arr, const size_t arr_size, const  T &neutral_) {
		buffer = (T*) calloc(arr_size + 1, sizeof(T));
		size = arr_size;
		neutral = neutral_;
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
 
	T get(int i) {
		T ret = neutral;
		for (; i >= 0; i = (i & (i + 1)) - 1) {
			ret = T_OP()(ret, buffer[i]);
		}
		return ret;
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
		return max(a, b);
	}
};
 
struct Node {
	long long x;
	long long i;
 
	Node() :
    x(0),
    i(0)
    {}
 
	Node(long long x_) {
		x  = x_;
		i = 0;
	}
 
	Node(long long x_, long long i_) {
		x = x_;
		i = i_;
	}
 
	bool operator<(const Node &other) {
        if (x > other.x) {
            return true;
        } else if (x < other.x) {
            return false;
        } else {
            return i < other.i;
        }
    }
};
 
int main() {
	int n = 0;
	scanf("%d", &n);
	Node *arr = (Node*) calloc(n, sizeof(Node));
	for (long long i = 0; i < n; ++i) {
		scanf("%lld", &arr[i].x);
		arr[i].i = i;
	}
 
    sort(arr, arr + n);
 
    vector<long long> dp(n, 0);
    FenvikTree<long long, functor_sum> fenv;
    fenv.ctor(nullptr, n, 0);
 
    for (int q = 0; q < n; ++q) {
        int x = arr[q].x;
        int i = arr[q].i;
 
        long long mx = fenv.get(i - 1);
        dp[i] = mx + 1;
        fenv.upd(i, mx + 1);
    }
 
    long long *buf = fenv.get_buffer();
    vector<long long> ans;
    int max_ind = 0;
    for (int i = 0; i < n; ++i) {
        if (dp[i] > dp[max_ind]) {
            max_ind = i;
        }
    }
 
    int prev_max_ind = max_ind;
    while (max_ind >= 0) {
        ans.push_back(max_ind + 1);
        prev_max_ind = max_ind;
        --max_ind;
        while (max_ind >= 0 && !(dp[max_ind] == dp[prev_max_ind] - 1 || arr[max_ind].x < arr[prev_max_ind].x)) { --max_ind;}
    }
 
    reverse(ans.begin(), ans.end());
    printf("%zu\n", ans.size());
    for (int i = 0; i < ans.size(); ++i) {
        printf("%lld ", ans[i]);
    }
    printf("\n");
 
	return 0;
}
 
// В изначально убитом массиве будем воскрешать элементы от наибольшего к наименьшему, тогда в фенвике на максимумах в очередной момент времени слева от нашей
// позиции лежит максимум по уже воскрешенным эл-там, т.е. тех и только тех, с кем мы можем войти в посл-ть. О(nlogn), иначе бы не зашло
