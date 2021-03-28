/*
Найти точки сочленения

Решение: ищем точки сочленения
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

void dfs(const MatrixMap<int> &g, const int v, vector<int> &tin, vector<int> &tout, vector<int> &used, int &dtime,
		 vector<int> &ret, set<int> &ans, const int p = -1) {
	if (used[v]) {
		return;
	}
	tin[v] = dtime++;
	ret[v] = tin[v];
	used[v] = 1;

	int us = 0;
	for (auto u : g[v]) {
		if (u == p) continue;

		if (used[u]) {
			ret[v] = min(ret[v], tin[u]);
		} else {
			dfs(g, u, tin, tout, used, dtime, ret, ans, v);
			++us;
			if (p != -1) {
				if (ret[u] >= tin[v]) {
					ans.insert(v);
				}
			} else if (us == 2) {
				ans.insert(v);
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
	set<int> ans;

	MatrixMap<int> g(n);
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		--a;
		--b;

		g[a].push_back(b);
		g[b].push_back(a);
	}

	for (int i = 0; i < n; ++i) {
		if (!used[i])
			dfs(g, i, tin, tout, used, dtime, ret, ans);
	}

	printf("%lu\n", ans.size());
	for (auto v : ans) {
		printf("%d ", v + 1);
	}
	printf("\n");

	return 0;
}
