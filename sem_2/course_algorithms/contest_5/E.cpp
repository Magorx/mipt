/*
Сложность задачи заключается только в том, чтобы ввести данные. В остальном все как обычно - из ненас. L запускаем дфс
по ориентированным ребрам. Делаем вид, что сами нашли парсоч
*/


#include <cstdlib>
#include <cstdio>
#include <vector>
#include <set>

using std::vector;
using std::set;

void dfs(const vector<set<int>> &g, int v, vector<int> &used) {
	if (used[v]) {
		return;
	}
	used[v] = true;

	for (auto u : g[v]) {
		dfs(g, u, used);
	}
}

int main() {
	int n, m;
	scanf("%d %d", &m, &n);

	vector<int> L(m);
	vector<int> R(n);
	vector<set<int>> g(m + n);

	for (int i = 0; i < m; ++i) {
		int x = 0;
		scanf("%d", &x);

		for (int j = 0; j < x; ++j) {
			int y = 0;
			scanf("%d", &y);
			--y;

			g[i].insert(m + y);
			// g[m + y].insert(i); - пока что ребра направляем только направо
		}
	}

	vector<int> matching(m + n, -1);
	for (int i = 0; i < m; ++i) {
		int x = 0;
		scanf("%d", &x);
		--x;

		if (x == -1) {
			continue;
		}

		matching[i] = m + x;
		matching[m + x] = i;

		g[i].erase(m + x);  // |
		g[m + x].insert(i); // | переворачивает ребра, входящие в парсоч
	}

	vector<int> used(m + n);
	for (int v = 0; v < m; ++v) {
		if (matching[v] == -1) {
			dfs(g, v, used); // запускаем дфс из ненасыщенных левых
		}			
	}

	vector<int> l_ans;
	vector<int> r_ans;
	for (int i = 0; i < m; ++i) {
		if (!used[i]) {
			l_ans.push_back(i);
		}
	}

	for (int i = 0; i < n; ++i) {
		if (used[m + i]) {
			r_ans.push_back(i);
		}
	}

	printf("%lu\n", l_ans.size() + r_ans.size());
	printf("%lu ", l_ans.size());
	for (auto x : l_ans) {
		printf("%d ", x + 1);
	}
	printf("\n");
	printf("%lu ", r_ans.size());
	for (auto x : r_ans) {
		printf("%d ", x + 1);
	}
	printf("\n");

	return 0;
}