/*
Пишем Прима, делая вид, что это не Дейкстра
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

const long long inf = 1000000000;

struct Pair {
	long long x;
	long long y;

	Pair() {}

	Pair(long long a, long long b):
	x(a),
	y(b)
	{}
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

long long mst_weight(vector<vector<Pair>> &g) {
	int n = g.size();
	vector<long long> dist(n, +inf);
	vector<Pair> parent(n);
	dist[0] = 0;

	set<Pair> q;
	for (int i = 0; i < n; ++i) {
		q.insert({dist[i], i});
	}

	while (q.size()) {
		Pair p = *q.begin();
		q.erase(p);

		int v = p.y;
		long long d = p.x;

		for (auto pp : g[v]) {
			int u = pp.y;
			long long c = pp.x;

			Pair cur_u(dist[u], u);
			if (q.find(cur_u) != q.end() && dist[u] > c) {
				q.erase(cur_u);
				dist[u] = c;
				parent[u] = {v, c};
				cur_u.x = dist[u];
				q.insert(cur_u);
			}
		}
	}

	long long ans = 0;
	for (int i = 1; i < n; ++i) {
		ans += parent[i].y;
	}

	return ans;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<vector<Pair>> g(n);

	for (int i = 0; i < m; ++i) {
		long long a, b, x;
		scanf("%lld %lld %lld", &a, &b, &x);
		--a;
		--b;
		g[a].push_back({x, b});
		g[b].push_back({x, a});
	}

	printf("%lld\n", mst_weight(g));

	return 0;
}
