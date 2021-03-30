/*
2-SAT на лампочках

Выражаем xor через or и and на бумажке, после чего соответствующие им ребра добавляем в граф
Забавно, что он у нас будет по факту неор., особенно за О(n + m)
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

void dfs(const MatrixMap<int> &g, const int v, vector<int> &used, const int use_mark = -1) {
	if (used[v]) {
		return;
	}
	used[v] = use_mark;

	for (auto u : g[v]) {
		if (!used[u]) dfs(g, u, used, use_mark);
	}
}

struct Triplex {
	int a;
	int b;
	int is;
};

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<int> used(2*m + 10,  0);
	vector<Triplex> bulbs(n + 10, {-1, -1, 0});

	MatrixMap<int> g(2 * m + 10);
	
	for (int i = 0; i < n; ++i) {
		int x;
		scanf("%d", &x);
		bulbs[i].is = (x == 1);
	}

	for (int i = 0; i < m; ++i) {
		int k;
		scanf("%d", &k);
		for (int j = 0; j < k; ++j) {
			int x;
			scanf("%d", &x);
			--x;

			if (bulbs[x].a >= 0) {
				bulbs[x].b = i;
			} else {
				bulbs[x].a = i;
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		Triplex x = bulbs[i];

		if (x.is) {
			g[2 * x.a].push_back(2 * x.b);
			g[2 * x.b].push_back(2 * x.a);
			g[2 * x.b + 1].push_back(2 * x.a + 1);
			g[2 * x.a + 1].push_back(2 * x.b + 1);
		} else {
			g[2 * x.b].push_back(2 * x.a + 1);
			g[2 * x.a].push_back(2 * x.b + 1);
			g[2 * x.b + 1].push_back(2 * x.a);
			g[2 * x.a + 1].push_back(2 * x.b);
		}
	}

	int cnt = 1;
	for (int i = 0; i < 2 * m; ++i) {
		if (!used[i]) {
			dfs(g, i, used, cnt);
			cnt++;
		}
	}

	for (int i = 0; i < m; ++i) {
		if (used[2 * i] == used[2 * i + 1]) {
			printf("NO\n");
			return 0;
		}
	}

	printf("YES\n");
	return 0;
}
