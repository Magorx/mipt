/*
Напишите программу, которая для двух вершин дерева определяет, является ли одна из них предком другой.
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
vector<bool> used;
int time;

void dfs(const MatrixMap<int> &g, const int v) {
	if (used[v]) {
		return;
	}
	used[v] = 1;

	tin[v] = time++;
	for (auto u : g[v]) {
		dfs(g, u);
	}

	tout[v] = time++;
}

int main() {
	int n, m;
	scanf("%d", &n);
	tin.resize (n, -1);
	tout.resize(n, -1);
	used.resize(n, 0);
	time = 1;

	int s = -1;
	MatrixMap<int> g(n);
	for (int i = 0; i < n; ++i) {
		int x;
		scanf("%d", &x);
		--x;
		if (x >= 0) {
			g[x].push_back(i);
		} else {
			s = i;
		}
	}

	dfs(g, s);

	scanf("%d", &m);
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		--a;
		--b;

		int a1 = tin[a], a2 = tout[a];
		int b1 = tin[b], b2 = tout[b];

		if (a1 < b1 && a2 > b2) {
			printf("1\n");
		} else {
			printf("0\n");
		}
	}

	return 0;
}
