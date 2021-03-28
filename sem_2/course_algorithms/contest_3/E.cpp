/*
Требуется найти количество ребер в конденсации ориентированного графа. Примечание: конденсация графа не содержит кратных ребер.

Используем Косарайойойо
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

template<typename T>
using Row = vector<T>;

template<typename T>
using MatrixMap = vector<Row<T>>;

void dfs(const MatrixMap<int> &g, const int v, vector<int> &tin, vector<int> &tout, vector<int> &used, int &dtime,
		 vector<int> &ans, const int use_mark = -1) {
	if (used[v]) {
		return;
	}
	used[v] = use_mark;

	tin[v] = dtime++;
	for (auto u : g[v]) {
		if (!used[u]) dfs(g, u, tin, tout, used, dtime, ans, use_mark);
	}

	tout[v] = dtime++;
	if (use_mark < 0) ans.push_back(v);
}

struct Pair {
	int x;
	int y;
};

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<int>  tin (n, -1);
	vector<int>  tout(n, -1);
	vector<int>  used(n,  0);
	int dtime = 1;
	vector<int> topsort;

	MatrixMap<int> g(n);
	MatrixMap<int> gr(n);
	vector<Pair> edges;
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		--a;
		--b;

		if (a == b | find(g[a].begin(), g[a].end(), b) != g[a].end()) {
			continue;
		}

		edges.push_back({a, b});
		g[a].push_back(b);
		gr[b].push_back(a);
	}

	for (int i = 0; i < n; ++i) {
		if (!used[i]) {
			dfs(g, i, tin, tout, used, dtime, topsort);
		}
	}

	reverse(topsort.begin(), topsort.end());
	for (int i = 0; i < used.size(); ++i) {
		used[i] = 0;
	}

	for (int v : topsort) {
		if (!used[v]) {
			dfs(gr, v, tin, tout, used, dtime, topsort, v + 1);
		}
	}

	vector<set<int>> bubik(n + 1);

	int cnt = 0;
	for (auto e : edges) {
		int x = used[e.x];
		int y = used[e.y];
		if (x == y || bubik[x].find(y) != bubik[x].end()) {
			continue;
		}

		cnt++;
		bubik[x].insert(y);
	}

	printf("%d\n", cnt);

	return 0;
}
