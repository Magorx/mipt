/*
Дейкстра еще разок. Будем, обновляясь из очередной вершины, перед этим делать вид, что мы замкнулись в цикл через
обрабатываемое ребро. Так мы переберем, фактически, все простые циклы, что и требуется!
Это решение пришло в голову и было сразу отброшено, ведь, очевидно, мы не успеем запустить дейкстру из каждой вершины
А через час размышлений я перечитал условие и понял, что, вообще-то, успеем
*/

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <queue>

using std::vector;
using std::priority_queue;
using std::set;
using std::min;

struct Pair {
	int x;
	int y;
	int used = 0;
	size_t idx;
};

bool operator<(const Pair &a, const Pair &b) {
	if (a.x < b.x) {
		return false;
	} else if (a.x > b.x) {
		return true;
	} else {
		return a.y > b.y;
	}
}

const long long inf = 10000000000;

long long djikstra(vector<vector<Pair>> &g, vector<vector<long long>> &matr, int s, int t) {
	long long mincyc = +inf;

	int n = g.size();
	vector<long long> dist(n, +inf);
	dist[s] = 0;
	priority_queue<Pair> q;
	q.push({0, s});

	while (q.size()) {
		Pair p = q.top();
		q.pop();

		int v = p.y;
		int d = p.x;

		if (dist[v] < d) {
			continue;
		}

		for (Pair &ed : g[v]) {
			if (ed.used == t) {
				continue;
			}

			int u = ed.x;
			int dd = ed.y;
			mincyc = min(mincyc, dist[u] + dist[v] + dd);
		}

		for (Pair &ed : g[v]) {
			int u = ed.x;
			int dd = ed.y;


			if (dist[u] > d + dd) {
				ed.used = t;
				g[u][ed.idx].used = t;

				dist[u] = d + dd;
				q.push({dist[u], u});
			}
		}
	}

	return mincyc;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<int> used(n, 0);
	int time = 0;

	vector<vector<Pair>> g(n);
	vector<vector<long long>> matr(n, vector<long long>(n, inf));

	for (int i = 0; i < m; ++i) {
		long long a, b, x;
		scanf("%lld %lld %lld", &a, &b, &x);
		--a;
		--b;
		g[a].push_back({b, x, 0, g[b].size()});
		g[b].push_back({a, x, 0, g[a].size() - 1});

		matr[a][b] = min(matr[a][b], x);
		matr[b][a] = min(matr[a][b], x);
	}

	long long mn = inf;
	for (int v = 0; v < n; ++v) {
		++time;
		mn = min(mn, djikstra(g, matr, v, time));
	}

	printf("%lld\n", mn);

	return 0;
}