#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>


std::vector<int> z_func(const char *s) {
    int len = strlen(s);
    std::vector<int> z(len);

    int l = 0;
    int r = 0;
    
    for (int i = 1; i < len; ++i) {
        z[i] = std::max(0, std::min(z[i - l], r - i));
        for (; i + z[i] < len && s[z[i]] == s[i + z[i]]; z[i]++);

        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}


int main() {
    char *s = (char*) malloc(1000001);
    scanf("%s", s);

    auto res = z_func(s);
    for (auto x : res) {
        printf("%d ", x);
    }
    printf("\n");

    free(s);

    return 0;
}