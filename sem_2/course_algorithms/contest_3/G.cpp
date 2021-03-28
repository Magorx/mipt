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
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		--a;
		--b;

		g[a].push_back(b);
		g[b].push_back(a);
	}

	int cnt = 1;
	for (int i = 0; i < n; ++i) {
		if (!used[i]) {
			dfs(g, i, tin, tout, used, dtime, topsort, cnt);
			cnt++;
		}
	}

	printf("%d\n", cnt - 1);
	for (int i = 0; i < n; ++i) {
		printf("%d ", used[i]);
	}
	printf("\n");

	return 0;
}
