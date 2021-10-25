#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <cmath>


using std::vector;
using std::string;
using std::cin;
using std::cout;


void prefsum(vector<int> &vec, int max_idx = -1) {
    if (max_idx >= 0) {
        max_idx = max_idx < vec.size() ? max_idx : vec.size();
    } else {
        max_idx = vec.size();
    }

    for (int i = 1; i < max_idx; ++i) {
        vec[i] += vec[i - 1];
    }
}

int get_second_comp(const vector<int> &ret, int cur_i, int l) {
    int x = ret[cur_i] - l;
    return x < 0 ? x + ret.size() : x;
}

bool same_eq(const vector<int> &eqs, int i1, int i2) {
    return eqs[i1] == eqs[i2];
}

void print_vec(const vector<int> &vec, const int delta = 1) {
    for (auto x : vec) {
        cout << x + delta << " ";
    }
    cout << "\n";
}


const int APLHABET_SIZE = 256;
vector<int> sufarr(const string &str) {
    int n = str.size();

    vector<int> ret(n);
    vector<int> eqs(n, 0);
    vector<int> pref(APLHABET_SIZE, 0); // for radix
    vector<int> new_ret(n);
    vector<int> new_eqs(n);

    for (int i = 0; i < n; ++i) {
        ++pref[str[i]];
    }
    prefsum(pref);

    for (int i = n - 1; i >= 0; --i) {
        ret[--pref[str[i]]] = i;
    }

    pref.resize(n);

    eqs[ret[0]] = 0;
    int cur_max_eq = 1;
    for (int i = 1; i < n; ++i) {
        if (str[ret[i]] != str[ret[i-1]])  ++cur_max_eq;
        eqs[ret[i]] = cur_max_eq - 1;
    }

    for (int l = 1; l < n; l *= 2) {
        // print_vec(ret);
        for (int i = 0; i < n; ++i) {
            new_ret[i] = get_second_comp(ret, i, l);
        }

        memset(&pref[0], 0, pref.size() * sizeof(int));

        for (int i = 0; i < n; ++i) {
            ++pref[eqs[new_ret[i]]];
        }
        prefsum(pref, cur_max_eq);

        for (int i = n - 1; i >= 0; --i) {
            ret[--pref[eqs[new_ret[i]]]] = new_ret[i];
        }
        new_eqs[ret[0]] = 0;

        cur_max_eq = 1;
        for (int i = 1; i < n; ++i) {
            int cur_comp = (ret[i - 0] + l) % n;
            int prv_comp = (ret[i - 1] + l) % n;

            if (!same_eq(eqs, ret[i], ret[i - 1]) || !same_eq(eqs, cur_comp, prv_comp)) {
                ++cur_max_eq;
            }

            new_eqs[ret[i]] = cur_max_eq - 1;
        }
        memcpy(&eqs[0], &new_eqs[0], n * sizeof(int));
    }

    return ret;
}


int main() {
    string str;
    cin >> str;
    str += (char) 0; // главное вот это вот не забыть....

    auto ans = sufarr(str);
    for (int i = 1; i < ans.size(); ++i) {
        cout << ans[i] + 1 << ' ';
    }
    cout << "\n";

}