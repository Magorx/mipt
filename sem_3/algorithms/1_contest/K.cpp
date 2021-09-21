#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>

std::vector<long long int> z_func(const char *s) {
    long long int len = strlen(s);
    std::vector<long long int> z(len);

    long long int l = 0;
    long long int r = 0;
    
    for (long long int i = 1; i < len; ++i) {
        z[i] = std::max(0ll, std::min(z[i - l], r - i));
        for (; i + z[i] < len && s[z[i]] == s[i + z[i]]; z[i]++);

        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

int main() {
    char *str = (char*) calloc(10000, sizeof(char));
    scanf("%s", str);

    size_t len = strlen(str);

    long long int ans = 1;
    long long int real_ans = 1;
    for (long long int suf_len = 2; suf_len <= len; ++suf_len) {
        auto r = z_func(str + len - suf_len);

        long long int mx = 0;
        for (long long int i = 0; i < r.size(); ++i) {
            mx = std::max(mx, r[i]);
        }

        ans += suf_len - mx;
        for (long long int i = mx + 1; i <= suf_len; ++i) {
            real_ans += i;
        }
    }

    printf("%lld\n", real_ans);

    return 0;
}
