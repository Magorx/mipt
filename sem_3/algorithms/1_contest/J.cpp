#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>


using std::cin;
using std::cout;
using std::string;
using std::vector;


vector<long long int> z_func(const std::string &s) {
    long long int len = s.length();
    vector<long long int> z(len, 0);

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
    string str;
    string pat;
    int k = 0;
    cin >> str >> pat >> k;

    int l_str = str.length();
    int l_pat = pat.length();

    auto s = pat + "#" + str + "@" + pat;
    auto zf = z_func(s); // straight z_f
    std::reverse(s.begin(), s.end());
    auto rf = z_func(s); // z_f for reversed

    int cnt = 0;
    vector<int> ans;

    for (int idx = 0; idx < l_str - l_pat + 1; ++idx) {
        int i = idx + l_pat + 1;
        int j = l_pat + l_str - idx - l_pat + 1;
        int match = zf[i] + rf[j];

        if (l_pat - match <= k) {
            ans.push_back(idx);
            cnt++;
        }
    }

    cout << cnt << '\n';
    for (auto x : ans) {
        cout << x + 1 << ' ';
    }
    cout << '\n';

    return 0;
}
