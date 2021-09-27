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

    int ret;
    int visited;

    TrieNode(int parent = 0, T own_letter = 0):
    to(),
    term(0),
    parent(parent),
    own_letter(own_letter),
    link(-1),
    transition(),
    ans_index(),
    ret(-1),
    visited(0)
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


template <typename T>
bool check(Trie<T> &bor, int v) { // we can't use code that can reach bad vertex by suff links
    if (!v) {
        return true;
    }

    TrieNode<T> &node = bor.data[v];
    if (node.term || !node.ret) {
        node.ret = 0;
        return false;
    } else {
        return check(bor, bor.get_link(v));
    }
}


template <typename T>
bool solvable(Trie<T> &bor, int v) {
    TrieNode<T> &node = bor.data[v];
    if (node.visited == 1) {
        return true;
    } else if (node.visited == 2) {
        return node.ret;
    }
    node.visited = 1;

    if (node.ret != -1) {
        node.visited = 2;
        return node.ret;
    }

    if (!check(bor, v)) {
        node.visited = 2;
        node.ret = 0;
        return node.ret;
    }

    int cur_ret = 0;
    for (char d = '0'; d <= '1'; ++d) {
        cur_ret |= solvable(bor, bor.next(v, d));
    }
    node.ret = cur_ret;

    node.visited = 2;
    return node.ret;
}


int main() {
    Trie<char> bor;

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

    printf("%s\n", solvable(bor, 0) ? "TAK" : "NIE");

    return 0;
}
