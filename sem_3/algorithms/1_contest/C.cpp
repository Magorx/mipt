#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <unordered_map>
#include <set>
#include <unordered_set>


using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::set;
using std::pair;
using std::unordered_set;


struct DNA {
    char *str;
    int idx;
    int v;

    DNA():
    str(nullptr),
    idx(-1)
    {}

    DNA(char *s, int i, int v):str(s),idx(i), v(v){}

    size_t operator()() const {
        return idx;
    }

    bool operator==(const DNA &other) const
    { return idx == other.idx;
    }

    bool operator<(const DNA &other) const
    { return idx < other.idx;
    }
};


namespace std {

  template <>
  struct hash<DNA>
  {
    std::size_t operator()(const DNA& k) const
    {
      using std::hash;
      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return hash<int>()(k.idx);
    }
  };

}


struct TrieNode {
    std::unordered_map<char, int> to;
    int term;

    set<DNA> to_proc;

    TrieNode(int parent = 0, char own_letter = 0):
    to(),
    term(0),
    to_proc()
    {}
};


struct Trie {    

    std::vector<TrieNode> data;
    vector<DNA> to_propel;

    Trie(const size_t init_size = 1):
    data(init_size),
    to_propel()
    {
        data.reserve(100000);
        to_propel.reserve(1000);
    }

    void add(const DNA &d, const size_t len) {
        size_t v = 0;
        char *s = d.str;
        vector<DNA> to_prop;
        for (size_t i = 0; i < len; ++i) {
            char c = s[i];
            if (data[v].to.find(c) == data[v].to.end()) {
                data.push_back(TrieNode(v, c));
                data[v].to[c] = data.size() - 1;
            }

            for (const auto x : data[v].to_proc) {
                if (*x.str == c) {
                    to_prop.push_back(x);
                }
            }

            for (auto x : to_prop) {
                x.v = v;
                to_propel.push_back(x);
                data[v].to_proc.erase(x);
            }
            to_prop.clear();
            
            v = data[v].to[c];
        }

        data[v].term++;
    }

    inline vector<int> prop_all() {
        vector<int> ret;
        for (int i = 0; i < to_propel.size(); ++i) {
            if (propel(to_propel[i])) {
                ret.push_back(to_propel[i].idx);
            }
        }
        to_propel.clear();
        
        return ret;
    }

    int propel(DNA &dna) {
        int v = dna.v;
        char c = *dna.str;

        while (c) {            
            if (data[v].term) {
                v = 0;
                continue;
            }

            if (data[v].to.find(c) != data[v].to.end()) {
                dna.str++;
                v = data[v].to[c];
                c = *dna.str;
                // printf("ok -> %d\n", v);
                continue;
            } else {
                data[v].to_proc.insert(dna);
                return 0;
            }
        }
        
        if (data[v].term) {
            return 1;
        } else {
            return false;
        }
    }
};


const int MOD = 10000;


int main() {
    Trie bor;

    int n = 0;
    int k = 0;

    scanf("%d", &n);

    int cnt = 0;

    char *str = (char*) malloc(1000005);
    getchar();
    for (int q = 0; q < n; ++q) {
        char c, cc = 0;
        scanf("%c%c%s", &c, &cc, str);
        getchar();
        // printf("c == |%c| str = |%s|\n", c, str);

        char *sd = strdup(str);

        int l = strlen(str);
        k %= l;
        if (k > 0) {
            memcpy(sd, str + k, l - k);
            memcpy(sd + l - k, str, k);
            // str = str.substr(k, str.length() - k) + str.substr(0, k);
        }

        // printf("c == |%c| str = |%s|\n", c, sd);

        
        if (c == '+') {
            bor.add({sd, cnt, 0}, l);
        } else {
            cnt++;
            bor.to_propel.push_back(DNA(sd, cnt, 0));
        }

        auto ans = bor.prop_all();
        k = ans.size();
        cout << k << ' ';
        for (auto x : ans) {
            cout << x << ' ';
        }
        cout << '\n';


        // bor.add(str, q);
    }

    return 0;
}
