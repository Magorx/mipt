/*
Нас просят найти эйлеров цикл
Ну, грех отказывать за О(кол-во ребер)
*/


#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stack>

using std::vector;
using std::stack;

struct Pair {
	int is;
	int used;
};

int main() {
	int n, s;
	scanf("%d %d", &n, &s);
	--s;

	vector<vector<Pair>> g(n, vector<Pair>(n, {0, 1}));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			int x;
			scanf("%d", &x);
			if (i != j && x == 0) g[i][j] = {1, 0}; else g[i][j] = {0, 1};
		}
	}

	stack<int> to_use;
	to_use.push(s);

	vector<int> ans;

	while (to_use.size()) {
		bool flag = false;
		int v = to_use.top();
		for (int u = 0; u < n; ++u) {
			Pair &e = g[v][u];
			if (e.is && !e.used) {
				flag = true;
				to_use.push(u);
				g[v][u].used = 1;
				break;
			}
		}
		
		if (!flag) {
			to_use.pop();
			ans.push_back(v + 1);
		}
	}

	for (int i = (int) ans.size() - 1; i > 0; --i) {
		printf("%d %d\n", ans[i], ans[i - 1]);
	}

	return 0;
}