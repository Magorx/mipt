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
	return a.x < b.x;
}

const long long inf = 1000000000;

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
	int n, k, m;
	scanf("%d %d %d", &n, &m, &k);

	vector<vector<Pair>> g(n);
	vector<int> fires(k);
	for (int i = 0; i < k; ++i) {
		scanf("%d", &fires[i]);
		--fires[i];
	}

	for (int i = 0; i < m; ++i) {
		int a, b, x;
		scanf("%d %d %d", &a, &b, &x);
		--a;
		--b;
		g[a].push_back({b, x});
		g[b].push_back({a, x});
	}

	int s, t;
	scanf("%d %d", &s, &t);
	--s;
	--t;

	//printf("!!!\n");

	auto dist = djikstra(g, t);
	long long min_f = inf;
	long long human = dist[s];

	// for (auto x : dist) {
	// 	printf("%lld ", x);
	// }
	// printf("\n");

	for (auto x : fires) {
		// printf("%lld ", x);
		min_f = std::min(min_f, dist[x]);
	}
	
	printf("%lld\n", min_f <= human ? -1 : human);

	return 0;
}