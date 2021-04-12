/*
Разбить на компоненты связанности

Ну короче копируем задачу E, раскомментируем дебаг вывод и отправляем
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

vector<int> dsu;
vector<int> dsu_size;

void dfs(const vector<set<int>> &g, const int v, vector<int> &tin, vector<int> &tout, vector<int> &used, int &dtime,
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
	if (use_mark < 0) ans.push_back(v); else (dsu[v] = -1);
}

int get(int v) {
  if (dsu[v] == v) {
    return v;
  } else {
    return dsu[v] = get(dsu[v]);
  }
}

void add(int first, int second) {
  first = get(first);
  second = get(second);
  if (dsu_size[first] > dsu_size[second]) {
    dsu[second] = first;
    dsu_size[first] += dsu_size[second];
  } else {
    dsu[first] = second;
    dsu_size[second] += dsu_size[first];
  }
}

struct Pair {
	int x;
	int y;
};

int main() {
	int n, m, k;
	scanf("%d %d %d", &n, &m, &k);
	vector<int>  tin (n, -1);
	vector<int>  tout(n, -1);
	vector<int>  used(n,  0);
	int dtime = 1;
	vector<int> topsort;

	dsu.resize(n + 1);
	dsu_size.resize(n + 1, 0);

	vector<set<int>> g(n);
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);

		g[a].insert(b);
		g[b].insert(a);
	}

	vector<Pair> es;
	for (int i = 0; i < k; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);

		es.push_back({a, b});
		g[a].erase(b);
		g[b].erase(a);
	}

	int cnt = 1;
	for (int i = 0; i < n; ++i) {
		if (!used[i]) {
			dfs(g, i, tin, tout, used, dtime, topsort, i + 1);
			cnt++;
		}
	}
	
	for (int i = 0; i < n; ++i) {
		dsu[i] = used[i] - 1;
	}
	--cnt;

	std::vector<int> ans;
	ans.push_back(cnt);

	for (int i = k - 1; i >= 0; --i) {
		Pair e = es[i];

		int x = e.x;
		int y = e.y;

		if (get(x) != get(y)) {
			--cnt;
		}
		add(x, y);

		ans.push_back(cnt);
	}

	for (int i = ans.size() - 1; i >= 0; --i) {
		printf("%d\n", ans[i]);
	}

	return 0;
}
