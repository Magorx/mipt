/*
Пишем Крускалу на дополненном наборе предложений, в который включили все ребра, смежные с мин вершиной
,делая вид, что до этого не писали два дфс-а и каких-то сложных конструкций, дебажа пол часа
*/

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>

using std::vector;
using std::min;

vector<long long> dsu;
vector<long long> dsu_size;

const long long inf = 100000000000000000;

struct DSU {
	vector<long long> data;
	vector<long long> prior;

	DSU(int n) {
		data.resize(n);
		prior.resize(n);

		for (long long i = 0; i < n; ++i) {
			data[i] = i;
			prior[i] = 1;
		}
	}

	long long get(long long v) {
	  if (data[v] == v) {
	    return v;
	  } else {
	    return data[v] = get(data[v]);
	  }
	}

	void unite(long long first, long long second) {
	  first = get(first);
	  second = get(second);
	  if (prior[first] > prior[second]) {
	    data[second] = first;
	    prior[first] += prior[second];
	  } else {
	    data[first] = second;
	    prior[second] += prior[first];
	  }
	}
};

struct Pair {
	long long x;
	long long y;
	long long w;
	long long used;

	Pair() {}

	Pair(long long a, long long b, long long c):
	x(a),
	y(b),
	w(c),
	used(0)
	{}

	Pair(long long a, long long b, long long c, long long d):
	x(a),
	y(b),
	w(c),
	used(d)
	{}
};

bool operator<(const Pair &a, const Pair &b) {
	if (a.w < b.w) {
		return true;
	} else if (a.w > b.w) {
		return false;
	} else {
		return a.y < b.y && a.x < b.x;
	}
}

void dsu_init(long long n) {
	dsu.resize(n);
	dsu_size.resize(n);
	
}

int main() {
	long long n, m;
	scanf("%lld %lld", &n, &m);
	DSU dsu(n);

	long long min_v = 0;
	long long min_v_d = +inf;
	vector<long long> arr(n);
	for (long long i = 0; i < n; ++i) {
		scanf("%lld", &arr[i]);
		if (arr[i] < min_v_d) { 
			min_v = i;
			min_v_d = arr[i];
		}
	}

	vector<Pair> edges;
	vector<vector<long long>> g(n);
	for (long long i = 0; i < m; ++i) {
		long long a, b, x;
		scanf("%lld %lld %lld", &a, &b, &x);
		--a;
		--b;

		edges.push_back({a, b, x});
	}

	for (int i = 0; i < n; ++i) {
		if (i == min_v) continue;
		else edges.push_back({i, min_v, arr[min_v] + arr[i]});
	}

	sort(edges.begin(), edges.end());

	long long ans = 0;
	for (auto e : edges) {
		if (dsu.get(e.x) != dsu.get(e.y)) {
			dsu.unite(e.x, e.y);
			ans += e.w;
		}
	}

	printf("%lld\n", ans);

	return 0;
}
