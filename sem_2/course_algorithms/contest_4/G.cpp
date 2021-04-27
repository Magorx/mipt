/*
Классная Дейкстра! Сделаем граф, номер вершин - модули числа а, ребра - переход с этажа по модулю равному i до этажа
по модулю равному (i + l) % a. Тогда дейкста даст нам номера первых этажей, на которых встречается очередной модуль.
И мы знаем, сколько еще раз мы сможем попасть на этот модуль - просто прибавляя а. Не забываем про огрызок, на 
а не поделившийся.
*/

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>

using std::vector;
using std::set;
using std::swap;

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

const long long inf = 9223372036854775800;

vector<long long> djikstra(const vector<vector<Pair>> &g, long long s, long long h) {
	long long n = g.size();
	vector<long long> dist(n, +inf);
	dist[s] = 0;
	set<Pair> q;
	q.insert({0, s});

	while (q.size()) {
		Pair p = *q.begin();
		q.erase(q.begin());

		long long v = p.y;
		long long d = p.x;

		if (dist[v] < d || dist[v] >= h) {
			continue;
		}

		for (auto ed : g[v]) {
			long long u  = ed.x;
			long long dd = ed.y;

			if (dist[u] > d + dd) {
				dist[u] = d + dd;
				q.insert({dist[u], u});
			}
		}
	}

	return dist;
}

int main() {
	long long h;
	scanf("%lld", &h);
	long long a, b, c;
	scanf("%lld %lld %lld", &a, &b, &c);

	if (b > a) {
		swap(a, b);
	}
	if (c > a) {
		swap(a, c);
	}

	// prlong longf("%lld %lld %lld\n", a, b, c);

	vector<vector<Pair>> g(a);

	for (long long i = 0; i < a; ++i) {
		long long x = (i + b) % a;
		long long y = (i + c) % a;

		g[i].push_back({x, b});
		g[i].push_back({y, c});
	}

	long long ans = 0;
	auto dist = djikstra(g, 0, h);

	long long h_div_a = h / a;
	long long h_mod_a = h % a;

	for (size_t x = 0; x < dist.size(); ++x) {
		long long d = dist[x];
		if (d >= h) {
			continue;
		}

		ans += h_div_a - d / a;
		if ((d % a) < h_mod_a) ans++;
	}
	
	printf("%lld\n", ans);

	return 0;
}