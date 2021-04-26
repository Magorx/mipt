/*
О, вот это интересная задача, потому что практически применимая. Заметим, что условие очень похоже на попытку найти
любой цикл, который даст нам выгоду. Собственно, используем для этого ФБ. Потом целый час дебажим его, туда-сюда, ничего не ясно
А потом заменяем double на long double и оно заходит. (Алгоритмические знания)++.
*/

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstdint>

using std::vector;
using std::min;
using std::max;

const long double inf = 0;

struct Edge
{
    long long x, y;
    long double rate;
    long double comm;
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
    long double v;
    scanf("%lld %lld %lld %Lg", &n, &m, &s, &v);
    --s;

    vector<long double> d(n, inf);
    vector<Edge> edges;
    for (long long i = 0; i < m; ++i) {
        long long a, b;
        long double rab, rba, cab, cba;
        scanf("%lld %lld %Lg %Lg %Lg %Lg", &a, &b, &rab, &cab, &rba, &cba);
        --a;
        --b;

        edges.push_back({a, b, rab, cab});
        edges.push_back({b, a, rba, cba});
    }

    d[s] = v;

    for (long long u = 0; u < 2*n; ++u) {
        for (Edge e : edges) {
            if (d[e.x] != inf) {
                d[e.y] = max(d[e.y], (d[e.x] - e.comm) * e.rate);
            }
        }
    }

    for (long long u = 0; u < n-1; ++u) {
        for (Edge e : edges) {
            if (d[e.x] != inf) {
                if (d[e.y] < (d[e.x] - e.comm) * e.rate) {
                    printf("YES\n");
                    return 0;
                }
            }
        }
    }

    if (d[s] > v) {
        printf("YES\n");
        return 0;
    }

    printf("NO\n");

    return 0;
}
