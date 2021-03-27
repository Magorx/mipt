/*
Дан ориентированный невзвешенный граф без петель и кратных рёбер. Необходимо определить есть ли в нём циклы, и если есть, то вывести любой из них.
*/

#include <cstdio>
#include <cstdlib>
#include <vector>

using std::vector;

template<typename T>
using Row = vector<T>;

template<typename T>
using MatrixMap = vector<Row<T>>;

vector<int>  tin;
vector<int>  tout;
vector<char> used;
int time;
bool flag = false;
bool to_add = false;
vector<int> ans;

void dfs(const MatrixMap<int> &g, const int v) {
	if (flag || used[v]) {
		return;
	}
	used[v] = 1;

	tin[v] = time++;
	for (auto u : g[v]) {
		if (flag) {
			break;
		}

		if (used[u] == 1) {
			flag = true;
			to_add = true;
			used[u] = 3;
			printf("YES\n");
			break;
		} else {
			dfs(g, u);
		}
	}

	if (to_add) {
		ans.push_back(v);
	}

	if (used[v] == 3) {
		to_add = false;
	}

	tout[v] = time++;
	used[v] = 2;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	tin.resize (n, -1);
	tout.resize(n, -1);
	used.resize(n, 0);
	time = 1;

	MatrixMap<int> g(n);
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		--a;
		--b;
		g[a].push_back(b);
	}

	for (int i = 0; i < n; ++i) {
		if (!used[i]) {
			dfs(g, i);
		}
		if (flag) {
			break;
		}
	}

	if (!flag) {
		printf("NO\n");
		return 0;
	}

	for (int i = ans.size() - 1; i >= 0; --i) {
		printf("%d ", ans[i] + 1);
	}
	printf("\n");

	return 0;
}
