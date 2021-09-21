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

    TrieNode(int parent = 0, T own_letter = 0):
    to(),
    term(0),
    parent(parent),
    own_letter(own_letter)
    {}
};

template <typename T>
struct Trie {

    std::vector<TrieNode<T>> data;

    Trie(const size_t init_size = 1):
    data(init_size)
    {}

    void add(const char *s) {
        add(s, strlen(s));
    }

    void add(const T *s, const size_t len) {
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

    std::vector<T> get_v(int v) {
        std::vector<T> ans;
        while (v) {
            ans.push_back(data[v].own_letter);
            v = data[v].parent;
        }
        return ans;
    }

    void print_sorted(size_t v, int **dot_cnt) { // thanks codeforces for ugly tasks
        if (data[v].term) {
            for (int q = 0; q < data[v].term; ++q) {
                for (int i = 0; i < **dot_cnt; ++i) {
                    printf(".");
                }
                *dot_cnt += 1;

                auto ans = get_v(v);
                for (int i = ans.size() - 1; i >= 0; --i) {
                    printf("%c", ans[i]);
                }
            }
        }

        for (auto key : data[v].to) {
            print_sorted(data[v].to[key.first], dot_cnt);
        }
    }
};


int main() {
    Trie<char> bor;

    char *inp = (char*) malloc(1000005);
    scanf("%s", inp);

    char *str = (char*) malloc(1000005);
    char *s = str;
    int  *dot_cnt = (int*) calloc(1000005, sizeof(int));
    int *dcnt = dot_cnt;

    int len = strlen(inp);
    for (int i = 0; i < len; ++i) {
        char c = inp[i];
        if (c == '.') {
            if (!*dcnt && i) {
                *s = '\0';
                s = str;
                bor.add(str);
            }
            
            *dcnt += 1;
        } else {
            if (*dcnt || i == 0) {
                ++dcnt;
            }

            *s = c;
            ++s;
        }
    }
    if (!*dcnt) {
        *s = '\0';
        s = str;
        bor.add(str);
    }

    dcnt = dot_cnt;
    bor.print_sorted(0, &dcnt);
    for (int i = 0; i < *dcnt; ++i) {
        printf(".");
    }
    printf("\n");

    free(inp);
    free(str);
    free(dot_cnt);
    
    return 0;
}