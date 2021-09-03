#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>

using std::vector;
using std::set;
using std::min;

vector<int> z_function (char *s) {
    int n = (int) strlen(s);
    vector<int> z (n);
    for (int i=1, l=0, r=0; i<n; ++i) {
        if (i <= r)
            z[i] = min (r-i+1, z[i-l]);
        while (i+z[i] < n && s[z[i]] == s[i+z[i]])
            ++z[i];
        if (i+z[i]-1 > r)
            l = i,  r = i+z[i]-1;
    }
    return z;
}

int main() {
    char *str = (char*) calloc(10000, sizeof(char));
    scanf("%s", str);

    size_t len = strlen(str);

    int ans = 0;
    for (int suf_len = 1; suf_len <= len; ++suf_len) {
        // printf("cur ans == %d---\n", ans);
        auto r = z_function(str + len - suf_len);

        set<int> s;
        int ll = r.size();
        for (int i = 0; i < ll; ++i) {
            s.insert(r[i]);
            // printf("%c - %d\n", str[len - suf_len + i], r[i]);
        }

        ans += suf_len - (s.size() == suf_len ? suf_len + 1 : 0) + (s.size() > 1 ? 1 : 0);
    }

    printf("%d\n", ans);

    return 0;
}