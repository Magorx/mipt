#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>


std::vector<int> manaker(const int *s, int n) {
    std::vector<int> ret(n);
    int left  =  0;
    int right = -1;

    for (int i = 0; i < n; ++i) {
        int win_size = 0;
        if (i <= right) {
            win_size = std::min(right - i + 1, ret[right - i + left + 1]);
        }

        for (;i + win_size < n && i - win_size - 1 >= 0 && s[i + win_size] == s[i - win_size - 1]; ++win_size); // aboba search
        ret[i] = win_size;

        if (i + win_size - 1 > right) {
            left  = i - win_size;
            right = i + win_size - 1;
        }
    } 

    return ret;
}

int main() {
    int n = 0;
    int m = 0;
    scanf("%d %d", &n, &m);
    
    int *s = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        scanf("%d", s + i);
    }

    auto arr = manaker(s, n);

    std::vector<int> ans;
    for (int i = 1; i < n; ++i) {
        if (arr[i] == i) {
            ans.push_back(i);
        }
    }

    for (int i = ans.size() - 1; i >= 0; --i) {
        printf("%d ", n - ans[i]);
    }
    printf("%d\n", n);

    return 0;
}