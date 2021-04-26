/*
Форд-Беллман он и на кф Форд-Беллман
*/

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstdint>

using std::vector;
using std::min;

const long long inf = 9223372036854775806ll;

struct Edge
{
    long long x, y, w;
};

void dfs(const vector<vector<long long>> &g, const long long v, vector<long long> &used) {
    if (used[v]) {
        return;
    }
    used[v] = 1;

    for (auto u : g[v]) {
        dfs(g, u, used);
    }
}

int main() {
    long long n, m, s;
    scanf("%lld %lld %lld", &n, &m, &s);
    --s;

    vector<long long>  used(n,  0);

    vector<vector<long long>> g(n);
    vector<long long> d(n, inf);
    vector<Edge> es;
    for (long long i = 0; i < m; ++i) {
        long long a, b, x;
        scanf("%lld %lld %lld", &a, &b, &x);
        --a;
        --b;

        g[a].push_back(b);
        es.push_back({a, b, x});
    }

    d[s] = 0;

    for (long long u = 0; u < n-1; ++u) {
        for (Edge e : es) {
            if (d[e.x] != inf) {
                d[e.y] = min(d[e.y], d[e.x] + e.w);
            }
        }
    }

    auto dd = d;

    for (long long u = 0; u < 1; ++u) {
        for (Edge e : es) {
            if (dd[e.x] != inf) {
                dd[e.y] = min(dd[e.y], dd[e.x] + e.w);
                if (dd[e.y] < d[e.y]) {
                    dfs(g, e.y, used);
                }
            }
        }
    }

    for (long long i = 0; i < n; ++i) {
        if (dd[i] < d[i]) {
            dfs(g, i, used);
        }
    }

    for (long long i = 0; i < n; ++i) {
        if (d[i] == inf) {
            printf("*\n");
        } else if (used[i]) {
            printf("-\n");
        } else {
            printf("%lld\n", d[i]);
        }
    }

    return 0;
}
