/*
Пишем Крускалу, делая вид, что что-то может быть приятнее dsu
*/

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <set>

using std::vector;
using std::reverse;
using std::find;
using std::set;
using std::min;

vector<long long> dsu;
vector<long long> dsu_size;

struct Pair {
	long long x;
	long long y;
	long long w;

	Pair() {}

	Pair(long long a, long long b, long long c):
	x(a),
	y(b),
	w(c)
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

void dsu_init(int n) {
	dsu.resize(n);
	dsu_size.resize(n);
	for (int i = 0; i < n; ++i) {
		dsu[i] = i;
		dsu_size[i] = 1;
	}
}

long long dsu_get(long long v) {
  if (dsu[v] == v) {
    return v;
  } else {
    return dsu[v] = dsu_get(dsu[v]);
  }
}

void dsu_unite(long long first, long long second) {
  first = dsu_get(first);
  second = dsu_get(second);
  if (dsu_size[first] > dsu_size[second]) {
    dsu[second] = first;
    dsu_size[first] += dsu_size[second];
  } else {
    dsu[first] = second;
    dsu_size[second] += dsu_size[first];
  }
}

int main() {
	long long n, m;
	scanf("%lld %lld", &n, &m);
	dsu_init(n);

	vector<Pair> g;
	for (long long i = 0; i < m; ++i) {
		long long a, b, x;
		scanf("%lld %lld %lld", &a, &b, &x);
		--a;
		--b;

		g.push_back({a, b, x});
	}

	sort(g.begin(), g.end());

	long long ans = 0;
	for (auto e : g) {
		if (dsu_get(e.x) != dsu_get(e.y)) {
			dsu_unite(e.x, e.y);
			ans += e.w;
		}
	}

	printf("%lld\n", ans);

	return 0;
}
