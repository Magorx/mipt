/*
Floid
*/

#include <cstdlib>
#include <cstdio>
#include <vector>
using std::vector;

int main() {
    int n;
    scanf("%d", &n);
    int inf = 10000000;
    vector<vector<long long>> g(n, vector<long long>(n, inf));
    vector<vector<long long>> d(n, vector<long long>(n, inf));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            long long x;
            scanf("%lld", &g[i][j]);
            d[i][j] = g[i][j];
        }
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (d[i][k] != inf && d[k][j] != inf) {
                    d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%lld ", d[i][j]);
        }
        printf("\n");
    }

    return 0;
}
