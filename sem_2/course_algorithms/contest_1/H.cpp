/*
Даны две последовательности чисел — a и b. Нужно найти наибольшую общую возрастающую подпоследовательность. Более формально: такие 1≤i1<i2<⋯<ik≤a.n и 1≤j1<j2⋯<jk≤b.n, что ∀t:ait=bjt,ait<ait+1 и k максимально.
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int inf = 99999999;

struct Pair {
    int x;
    int y;
};
 
int main() {
    int n = 0;
    int m = 0;
    scanf("%d %d", &n, &m);

    vector<long long> a(n + 1);
    vector<long long> b(m + 1);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &a[i + 1]);
    }
    for (int i = 0; i < m; ++i) {
        scanf("%lld", &b[i + 1]);
    }

    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, 0));
    long long ans = 0;
    for (int i = 1; i <= n; ++i) {
        long long cur_max = 0;
        for (int j = 1; j <= m; ++j) {
            dp[i][j] = dp[i - 1][j];
            if (a[i] == b[j]) {
                dp[i][j] = max(dp[i][j], cur_max + 1);
            }

            if (a[i] > b[j]) {
                cur_max = max(cur_max, dp[i - 1][j]);
            }

            ans = max(ans, dp[i][j]);
        }
    }

    printf("%lld\n", ans);
 
    return 0;
}

// dp[i][j] - макс длина общ возр подпосл при рассмотрении первой строки по i-й символ, второй по j-й
// тогда ясно, как обновлять dp, идя по очередному символу в первой строке. O(n * m)