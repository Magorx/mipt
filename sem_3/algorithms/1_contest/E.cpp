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

    std::vector<int> ans_index;

    TrieNode(int parent = 0, T own_letter = 0):
    to(),
    term(0),
    parent(parent),
    own_letter(own_letter),
    link(-1),
    transition(),
    ans_index()
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
        data[v].ans_index.push_back(ans_index);
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


int main() {
    Trie<char> bor;

    char *inp = (char*) malloc(1000005);
    scanf("%1000004s", inp);

    int n = 0;
    scanf("%d", &n);

    char *str = (char*) malloc(1000005);
    std::vector<int> lens(n);
    for (int q = 0; q < n; ++q) {
        scanf("%1000004s", str);
        bor.add(str, q);
        lens[q] = strlen(str);
    }
    free(str);

    int l = strlen(inp);
    int v = 0;
    std::vector<std::vector<int>> ans(n);
    for (int i = 0; i < l; ++i) {
        char c = inp[i];

        v = bor.next(v, c);

        int cur = v;
        while (cur) {
            if (bor.data[cur].term) {
                auto ans_idx = bor.data[cur].ans_index;
                
                for (auto idx : ans_idx) { // **** cf for ***** tasks
                    ans[idx].push_back(i - lens[idx] + 2);
                }
            }

            int link = bor.get_link(cur);
            cur = link;
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%ld ", ans[i].size());
        for (auto x : ans[i]) {
            printf("%d ", x);
        }
        printf("\n");
    }

    free(inp);

    return 0;
}
