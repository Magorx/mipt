#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>


template <typename T>
struct TrieNode {
    std::map<char, int> to;
    int term;
    
    int parent;
    T own_letter;

    int link;
    std::map<char, int> transition;

    int cnt;
    int cnt_prev;

    TrieNode(int parent = 0, T own_letter = 0):
    to(),
    term(0),
    parent(parent),
    own_letter(own_letter),
    link(-1),
    transition(),
    cnt(0),
    cnt_prev(0)
    {}
};

template <typename T>
struct Trie {

    std::vector<TrieNode<T>> data;

    Trie(const size_t init_size = 1):
    data(init_size)
    {}

    void add(const char *s, int ans_index) {
        add(s, strlen(s), ans_index);
    }

    void add(const T *s, const size_t len, int ans_index) {
        size_t v = 0;
        for (size_t i = 0; i < len; ++i) {
            if (data[v].to.find(s[i]) == data[v].to.end()) {
                data.push_back(TrieNode<T>(v, s[i]));
                data[v].to[s[i]] = data.size() - 1;
            }
            
            v = data[v].to[s[i]];
        }

        data[v].term++;
    }

    int get_link(int v) {
        if (data[v].link == -1) {
            if (!v || !data[v].parent) {
                data[v].link = 0;
            } else {
                data[v].link = next(get_link(data[v].parent), data[v].own_letter);
            }
        }
        
        return data[v].link;
    }

    int next(int v, char c) {
        if (data[v].transition.find(c) == data[v].transition.end()) {
            if (data[v].to.find(c) != data[v].to.end()) {
                data[v].transition[c] = data[v].to[c];
            } else {
                if (!v) {
                    data[v].transition[c] = 0;
                } else {
                    data[v].transition[c] = next(get_link(v), c);
                }
            }
        }

        return data[v].transition[c];
    }
};


template <typename T>
bool term_linked(Trie<T> &bor, int v) { // we can't use code that can reach bad vertex by suff links
    if (!v) {
        return false;
    }

    TrieNode<T> &node = bor.data[v];
    if (node.term) {
        return true;
    } else {
        return term_linked(bor, bor.get_link(v));
    }
}


const int MOD = 10000;


int main() {
    Trie<char> bor;

    int n = 0;
    int k = 0;
    scanf("%d %d", &n, &k);

    char *str = (char*) malloc(105);
    for (int q = 0; q < k; ++q) {
        scanf("%104s", str);
        bor.add(str, q);
    }
    free(str);


    bor.data[0].cnt = 1;

    for (int q = 0; q < n; ++q) {
        for (int v = 0; v < bor.data.size(); ++v) {
            bor.data[v].cnt_prev = bor.data[v].cnt;
        }

        for (int v = 0; v < bor.data.size(); ++v) {
            if (term_linked(bor, v)) continue;

            for (char c = 'a'; c <= 'z'; ++c) {
                int vn = bor.next(v, c);

                bor.data[vn].cnt += bor.data[v].cnt_prev;
                bor.data[vn].cnt %= MOD;
            }
        }

        for (int v = 0; v < bor.data.size(); ++v) {
            bor.data[v].cnt -= bor.data[v].cnt_prev;
            bor.data[v].cnt = bor.data[v].cnt;
        }
    }

    int mx = 1;
    for (int i = 0; i < n; ++i) {
        mx = (mx * 26) % MOD;
    }

    int ans = 0;
    for (int v = 0; v < bor.data.size(); ++v) {
        if (!term_linked(bor, v)) ans = (ans + bor.data[v].cnt + MOD) % MOD;
    }

    printf("%d\n", ((mx - ans) + 2 * MOD) % MOD);

    return 0;
}
