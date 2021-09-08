#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include <map>

using std::vector;
using std::set;
using std::min;
using std::map;

char invchar(char n) {
    if (n == '1') {
        return '0';
    } else {
        return '1';
    }
}

struct Node {
    Node *children[2];
    bool is_term;
    bool has_child;

    Node() :
    children(),
    is_term(false),
    has_child(false)
    {}

    void insert(const char *str) {
        Node *cur = this;

        while (*str) {
            if (cur->children.find(*str) == cur->children.end()) {
                cur->children[*str] = new Node();
            }

            cur = cur->children[*str];

            ++str;
        }
    }

    void find(const char *str, char *out) {
        Node *cur = this;

        while (*str) {
            if (cur->children.find(*str) == cur->children.end()) {
                *out = invchar(*str);
                cur = cur->children[invchar(*str)];
            } else {
                *out = *str;
                cur = cur->children[*str];
            }

            ++str;
            ++out;
        }
    }

    void del(const char *str) {
        if (!*str) {
            return;
        }

        if (children.find(*str) == children.end()) {
            return;
        }

        children[*str]->del(str + 1);

        if (children[*str]->children.size() == 0) {
            delete children[*str];
            children.erase(*str);
        }
    }
};

const unsigned SIZE = 33;

char *int_to_str(unsigned n) {
    char* str = (char*) calloc(SIZE + 1, sizeof(char));
    for (unsigned i = 0; i < SIZE; ++i) {
        str[i] = '0';
    }

    unsigned i = SIZE;

    if (!n) {
        str[--i] = '0';
        return str;
    }

    while (n) {
        str[--i] = '0' + n % 2;
        n /= 2;
    }

    return str;
}

void invstr(char *str) {
    while (*str) {
        *str = invchar(*str);
        ++str;
    }
}

unsigned str_to_int(char *str) {
    unsigned n = 0;
    for (int i = SIZE; i >= 0; --i) {
        // printf("i = %d -> %d\n", i, n);
        if (str[i] == '1') {
            n += (1 << (SIZE - i - 1));
        }
    }
    return n;
}

int main() {
    Node bor;

    unsigned q = 0;
    scanf("%u", &q);
    while (q--) {
        unsigned type = 0, n = 0;
        scanf("%u %u", &type, &n);
        
        char *s = int_to_str(n);
        // printf("%s -> %d\n", s, str_to_int(s));
        char* out = (char*) calloc(65, sizeof(char));

        if (type == 1) {
            bor.insert(s);
        } else if (type == 2) {
            bor.del(s);
        } else {
            // printf("%s -> ", s);
            invstr(s);
            // printf("%s\n", s);
            bor.find(s, out);
            printf("%u\n", str_to_int(out));
            // printf("%s|%s|\n", out, s);
        }

        free(s);
        free(out);
    }

    return 0;
}
