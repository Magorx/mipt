/*
У нас есть несколько предметов и рюкзак, который выдерживает вес C. Предмет с номером i имеет вес xi. Определите число различных наборов предметов, которые можно унести в рюкзаке. Два набора считаются различными, если существует хотя бы один предмет, который включён в один из наборов и не включён в другой.
*/
 
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
 
using namespace std;
 
int high_one(long long x) {
    int cnt = 0;
    while (x) {
        cnt += 1;
        x >>= 1;
    }
    return cnt - 1;
}
 
int main() {
	int n = 0;
	scanf("%d", &n);
 
	int m = n / 2;
    n = n - m;
 
    vector<long long> arr1(n, 0);
    vector<long long> arr2(m, 0);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &arr1[i]);
    }
    for (int i = 0; i < m; ++i) {
        scanf("%lld", &arr2[i]);
    }
 
    int c = 0;
    scanf("%d", &c);
 
    vector<long long> dp1(1 << n, 0);
    unsigned int p2 = 1 << n;
    for (int i = 1; i < p2; ++i) {
        dp1[i] = dp1[i - (1 << high_one(i))] + arr1[high_one(i)];
        // printf("%d) h[%d] %lld\n", i, high_one(i), dp1[i]);
    }
 
    sort(dp1.begin(), dp1.end());
 
 
    vector<long long> dp2(1 << m, 0);
    p2 = 1 << m;
    long long cnt = 0;
    for (int i = 0; i < p2; ++i) {
        if (i != 0) dp2[i] = dp2[i - (1 << high_one(i))] + arr2[high_one(i)];
        // printf("%d) h[%d] %lld\n", i, high_one(i), dp2[i]);
 
        int l = -1;
        int r = dp1.size();
        while (r - l > 1) {
            int mid = (l + r) / 2;
            if (dp1[mid] + dp2[i] <= c) {
                l = mid;
            } else {
                r = mid;
            }
        }
 
        cnt += l + 1;
    }
 
    printf("%d\n", cnt);
 
	return 0;
}
 
// bitmasks + MITM = O(2^(n/2) + n * log(n))
