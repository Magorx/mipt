/*
Кун-Кун - наш лучший друг в починке мостов, т.к. им мы поймем оптимальное расположение дощечек 2х1
(если, конечно, дощечки 1х1 ставить просто не выгодней)
*/


#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>

using std::max;
using std::min;
using std::vector;
using std::set;

bool agument(const vector<vector<int>> &g, int v, vector<int> &used, vector<int> &matching) {
	if (used[v]) {
		return false;
	}
	used[v] = true;

	for (auto u : g[v]) {
		if (matching[u] == -1 || agument(g, matching[u], used, matching)) return (matching[u] = v) + 1;
	}
	return false;
}

vector<int> get_max_matching(const vector<vector<int>> &g, int m) {
	int n = g.size();
	vector<int> matching(n, -1);
	
	vector<int> used(n, 0);
	for (int i = 0; i < n; ++i) {
		if ((i / m + i % m) % 2) {
			continue;
		}

		for (int i = 0; i < n; ++i) {
			used[i] = 0;
		}

		agument(g, i, used, matching);
	}

	return matching;
}

int get_v(int n, int m, int i, int j) {
	return i * m + j;
}

bool check_coord(int n, int m, int i, int j) {
	return i >= 0 && j >= 0 && i < n && j < m;
}

int main() {
	int n, m;
	long long a, b;
	scanf("%d %d %lld %lld\n", &n, &m, &a, &b);

	vector<vector<char>> most(n + 1, vector<char>(m + 1, 0));
	char c;
	long long free = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			scanf("%c", &c);
			most[i][j] = c == '*';
			free += c == '*';
		}
		scanf("%c", &c);
	}

	if (2 * b <= a) {
		printf("%lld\n", free * b);
		return 0;
	}
	
	vector<vector<int>> g(n * m); // преобразуем в явный граф, потому что можем
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (!most[i][j]) {
				continue;
			}
			int v = get_v(n, m, i, j);

			int ii = i;
			int jj = j;
			int vv = get_v(n, m, ii, jj);

			ii = i - 1;
			vv = get_v(n, m, ii, jj);
			if (check_coord(n, m, ii, jj) && most[ii][jj]) {
				g[v].push_back(vv);
			}
			ii = i;

			ii = i + 1;
			vv = get_v(n, m, ii, jj);
			if (check_coord(n, m, ii, jj) && most[ii][jj]) {
				g[v].push_back(vv);
			}
			ii = i;

			jj = j - 1;
			vv = get_v(n, m, ii, jj);
			if (check_coord(n, m, ii, jj) && most[ii][jj]) {
				g[v].push_back(vv);
			}
			jj = j;

			jj = j + 1;
			vv = get_v(n, m, ii, jj);
			if (check_coord(n, m, ii, jj) && most[ii][jj]) {
				g[v].push_back(vv);
			}
			jj = j;
		}
	}

	auto matching = get_max_matching(g, m);
 
	long long cnt = 0;
	for (auto x : matching) {
		cnt += x != -1;
	}
 
	printf("%lld\n", cnt * a + (free - cnt * 2) * b);

	return 0;
}
