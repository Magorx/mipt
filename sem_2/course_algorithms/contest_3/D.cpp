/*
topsort
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
vector<int> topsort;

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
			break;
		}

		dfs(g, u);
	}

	tout[v] = time++;
	used[v] = 2;
	topsort.push_back(v);
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

	if (flag) {
		printf("-1\n");
		return 0;
	}

	for (int i = topsort.size() - 1; i >= 0; --i) {
		printf("%d ", topsort[i] + 1);
	}
	printf("\n");

	return 0;
}
