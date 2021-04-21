/*
Дейкстра. Решается удалением 20ти строк из задачи про спелеологов и костры
*/

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>

using std::vector;
using std::set;

struct Pair {
	long long x;
	long long y;
};

bool operator<(const Pair &a, const Pair &b) {
	if (a.x < b.x) {
		return true;
	} else if (a.x > b.x) {
		return false;
	} else {
		return a.y < b.y;
	}
}

const long long inf = 1000000000000;

vector<long long> djikstra(const vector<vector<Pair>> &g, int s) {
	int n = g.size();
	vector<long long> dist(n, +inf);
	dist[s] = 0;
	vector<char> used(n, 0);
	set<Pair> q;
	q.insert({0, s});

	while (q.size()) {
		Pair p = *q.begin();
		q.erase(q.begin());

		int v = p.y;
		int d = p.x;

		if (dist[v] < d) {
			continue;
		}

		for (auto ed : g[v]) {
			int u = ed.x;
			int dd = ed.y;

			if (dist[u] > d + dd) {
				dist[u] = d + dd;
				q.insert({dist[u], u});
			}
		}
	}

	return dist;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);

	vector<vector<Pair>> g(n);
	int s, t;
	scanf("%d %d", &s, &t);
	--s;
	--t;

	for (int i = 0; i < m; ++i) {
		long long a, b, x;
		scanf("%lld %lld %lld", &a, &b, &x);
		--a;
		--b;
		g[a].push_back({b, x});
		g[b].push_back({a, x});
	}

	auto dist = djikstra(g, s);
	
	printf("%lld\n", dist[t] == inf ? -1 : dist[t]);

	return 0;
}