// ������� ������������ ��� ������, ������� ����� ������ � ������� ���������������� S, ���� ���� N ������� ������� � ��������� ������.
 
#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
int main() {
    long long W = 0;
    int n = 0;
    scanf("%lld %d", &W, &n);
    ++W;
    vector<long long> arr(n + 1, 0);
 
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &arr[i + 1]);
    }
 
    vector<vector<long long>> dp(n + 1, vector<long long>(W, -1));
    dp[0][0] = 0;
 
    for (int i = 0; i < n; ++i) {
        for (int w = 0; w < W; ++w) {
            dp[i + 1][w] = max(dp[i + 1][w], dp[i][w]);
            if (w + arr[i + 1] < W) {
                dp[i + 1][w + arr[i + 1]] = max(dp[i + 1][w + arr[i + 1]], dp[i][w] + arr[i + 1]);
            }
        }
    }
 
    long long ans = 0;
    for (int w = 0; w < W; ++w) {
        ans = max(ans, dp[n][w]);
    }
 
    printf("%lld\n", ans);
 
    return 0;
}
 
// ���������� �� dp[i][w] - ���� ��������� ��������� �� ������ i ��������� ����� w. �������� �� O(W * n)
