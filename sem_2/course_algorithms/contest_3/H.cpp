/*
Найти мосты

Решение: ищем мосты
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
using std::max;

template<typename T>
using Row = vector<T>;

template<typename T>
using MatrixMap = vector<Row<T>>;

struct Pair {
	int x;
	int y;

	Pair(int a, int b):
	x(min(a, b)),
	y(max(a, b))
	{}
};

bool operator<(const Pair &a, const Pair &b) {
	if (a.x < b.x) {
		return true;
	} else if (a.x > b.x) {
		return false;
	} else {
		return a.y < b.y;
	}
}

void dfs(const MatrixMap<int> &g, const int v, vector<int> &tin, vector<int> &tout, vector<int> &used, int &dtime,
		 vector<int> &ret, set<Pair> &ans, const int p = -1) {
	if (used[v]) {
		return;
	}
	tin[v] = dtime++;
	ret[v] = tin[v];
	used[v] = 1;

	for (auto u : g[v]) {
		if (u == p) continue;

		if (used[u]) {
			ret[v] = min(ret[v], ret[u]);
		} else {
			dfs(g, u, tin, tout, used, dtime, ret, ans, v);
			if (ret[u] >= tin[u]) {
				ans.insert({v, u});
			}
			ret[v] = min(ret[v], ret[u]);
		}
	}

	tout[v] = dtime++;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<int>  tin (n, -1);
	vector<int>  tout(n, -1);
	vector<int>  used(n,  0);
	vector<int>  ret (n,  -1);
	int dtime = 1;
	set<Pair> ans;

	MatrixMap<int> g(n);
	std::vector<Pair> edges;
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		--a;
		--b;

		g[a].push_back(b);
		g[b].push_back(a);

		edges.push_back({a, b});
	}

	for (int i = 0; i < n; ++i) {
		if (!used[i])
			dfs(g, i, tin, tout, used, dtime, ret, ans);
	}

	printf("%lu\n", ans.size());
	for (int i = 0; i < m; ++i) {
		Pair e = edges[i];
		if (ans.find(e) != ans.end()) {
			printf("%d\n", i + 1);
		}
	}

	return 0;
}
