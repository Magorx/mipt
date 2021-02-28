// ���� ������������������ ����� ����� ����� n. ����� ����� ���������� ��������� �������� ����������������������
 
#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
const long long M = 1000000007;
 
int main() {
    int n = 0;
    scanf("%d", &n);
    vector<long long> arr(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &arr[i + 1]);
    }
 
    vector<long long> dp(n + 1, 0);
    vector<long long> last_pos(n + 1, -1);
    dp[0] = 1;
 
    for (int i = 1; i <= n; ++i) {
        long long x = arr[i];
 
        dp[i] = (dp[i - 1] * 2) % M;
        if (last_pos[x] > -1) {
            dp[i] = (dp[i] - dp[last_pos[x] - 1] + M) % M;
        }
 
        last_pos[x] = i;
    }
 
    printf("%lld\n", dp[n] - 1);
 
    return 0;
}
 
// ��������� ����� �� ����� ����� � ������� �� ����� �������, ��� ���� �� ���, �� ���� �� ������ ������� ��, � ������� �� ������������� �� ��� �� ����� � ������� ���
