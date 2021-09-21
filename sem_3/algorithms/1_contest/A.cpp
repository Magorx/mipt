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
    char *s = (char*) malloc(50005);
    char *t = (char*) malloc(50005);
    s[0] = '#';
    scanf("%s", s + 1);
    scanf("%s", t);
    int len = strlen(t);
    char *text = strcat(t, s);

    auto res = z_func(text);
    for (int i = len; i < res.size(); ++i) {
        if (res[i] == len) {
            printf("%d ", i - len - 1);
        }
    }
    printf("\n");

    free(s);
    free(t);

    return 0;
}