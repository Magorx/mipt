#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);

    int *arr = (int*) calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }

    int *dp = (int*) calloc(n, sizeof(int));
    int *parents = (int*) calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i) {
        parents[i] = -1;
    }
    int max_len = 0;
    int max_index = 0;

    for (int i = 0; i < n; ++i) {
        dp[i] = 1;
        parents[i] = -1;
        for (int j = 0; j < i; ++j) {
            if (arr[j] < arr[i]) {
                if (dp[i] < dp[j] + 1) {
                    dp[i] = dp[j] + 1;
                    parents[i] = j;

                    if (dp[i] > max_len) {
                        max_len = dp[i];
                        max_index = i;
                    }
                }
            }
        }
    }

    int cur = max_index;
    vector<int> ans;
    while (cur != -1) {
        ans.push_back(cur);
        cur = parents[cur];
    }
    reverse(ans.begin(), ans.end());

    printf("%d\n", ans.size());
    for (auto x : ans) {
        printf("%d ", x);
    }
    printf("\n");

    return 0;
}