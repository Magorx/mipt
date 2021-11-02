#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
#include <set>


#define ll int


const ll ALPHABET_SIZE = 27;



template <typename KEY_T, typename TASK_DATA> 
class SuffixAutomat {
public:
    struct Node;
    using NodePtr = ll;

private:
    Node *new_node(const KEY_T &key = 0, NodePtr prev = -1) {
        nodes.push_back({this, (ll) nodes.size(), key});
        Node *ret = &nodes[nodes.size() - 1];

        if (prev >= 0) {
            Node *prev_node = node(prev);
            ret->len = prev_node->len + 1;
            ret->l   = prev_node->l + 1;
            ret->r   = prev_node->r + 1;
        }

        return ret;
    }

    Node *new_node(NodePtr prev) {
        nodes.push_back({});
        Node *ret = &nodes[nodes.size() - 1];
        *ret = *node(prev);

        ret->idx = nodes.size() - 1;
        ret->len++;

        return ret;
    }


    void add_key(const KEY_T &key) {
        #define RETURN do {last = cur->idx; return; } while(false)

        auto cur = new_node(key, last);

        auto upper = node(last);
        for (; upper && !upper->has_next(key); upper = node(upper->link)) {
            upper->next[key] = cur->idx;
        }

        if (!upper) {
            cur->link = root;
            RETURN;
        }

        Node *child = upper->go(key);
        if (upper->len + 1 == child->len) {
            cur->link = child->idx;
            child->r = cur->r;
            RETURN;
        }

        ll upper_idx = upper->idx;
        ll cur_idx = cur->idx;
        ll child_idx = child->idx;

        auto tmp = new_node(child->idx); // invalidates current Node*
        upper = node(upper_idx);
        cur   = node(cur_idx);
        child = node(child_idx);

        tmp->r = cur->r;
        tmp->len = upper->len + 1;

        for (; upper && upper->has_next(key) && upper->next[key] == child->idx; upper = node(upper->link)) {
            upper->next[key] = tmp->idx;
        }

        child->link = cur->link = tmp->idx;
        RETURN;

        #undef RETURN
    }

public:

    std::vector<Node> nodes;
    NodePtr last;

    struct Node {
        SuffixAutomat *automat;
        NodePtr idx;

        KEY_T c = 0;
        ll len = 0;
        
        NodePtr link = -1;
        NodePtr next[ALPHABET_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        
        ll l = 0;
        ll r = 0;

        bool terminal = false;
        bool visited = false;

        TASK_DATA data;

        char get_first(char *str, int n) {
            int rev_r = r - 1;
            int rev_l = rev_r - len + 1;

            int l = n - rev_r - 1;
            int r = n - rev_l - 1;

            l = r - ((len) - node(link)->len) + 1;
            r += 1;

            return str[l];
        }

        ll task_update(char *str, int l) {
            if (data.dp != -1) {
                return data.dp;
            }
            if (link != -1) {
                node(link)->data.to_visit.insert({get_first(str, l), idx});
            }

            data.dp = 0;
            for (ll c = 0; c < ALPHABET_SIZE; ++c) {
                if (!has_next(c)) continue;

                Node *v = go(c);
                data.dp += v->task_update(str, l);

                if (!v->data.p || v->data.p->len < len) {
                    v->data.p = this;
                }

                data.cnt += v->data.cnt;
            }
            data.dp += data.cnt;

            // if (link != -1) node(link)->data.prior = node(link)->data.prior || data.prior;
            
            return data.dp;
        }

        bool is_leaf() {
            return 0;
        }

        bool has_next(const KEY_T &key) { 
            return next[key] != -1;
        }

        Node *go(const KEY_T &key) {
            if (has_next(key)) {
                return automat->node(next[key]);
            } else {
                return nullptr;
            }
        }

        Node *node(NodePtr idx) {
            return automat->node(idx);
        }
    };


public:
    NodePtr root;

    SuffixAutomat(ll nodes_to_reserve = 100000) :
    nodes(),
    last(0),
    root(0)
    {
        nodes.reserve(nodes_to_reserve);
        root = last = new_node()->idx;
    }

    Node *node(NodePtr idx) {
        if (idx == -1) {
            return nullptr;
        }

        return &nodes[idx];
    }

    void put_terminals() {
        auto v = node(last);
        while (v) {
            v->terminal = true;
            v->data.cnt = 1;
            v = node(v->link);
        }
    }

    void add_string(const KEY_T *str) {
        ll l = strlen(str);

        for (ll i = 0; i < l; ++i) {
            add_key(str[i] - 'a');
        }
        put_terminals();
    }

    bool contatins(const KEY_T *str) {
        ll l = strlen(str);

        Node *cur = node(root);
        for (ll i = 0; i < l; ++i) {
            cur = cur->go(str[i]);
            if (!cur) {
                return false;
            }
        }

        return true;
    }

    void print_max(Node *node) {
        for (ll c = 'z'; c >= 'a'; --c) {
            if (node->has_next(c - 'a')) {
                printf("%c", c);
                print_max(node->go(c - 'a'));
                return;
            }
        }
    }

    bool get_nth(Node *node, ll n, std::vector<char> &ans) {
        if (!node) return false;

        if (n <= node->data.cnt) {
            for (auto x : ans) {
                printf("%c", x);
            }
            printf("\n");
            return true;
        }

        bool ret = false;
        for (ll i = 0; i < ALPHABET_SIZE; ++i)
        {
            if (node->has_next(i) && n - node->data.cnt <= node->go(i)->data.dp) {
                ans.push_back('a' + i);
                ret = ret || get_nth(node->go(i), n - node->data.cnt, ans);
                ans.pop_back();
            }
    
            if (node->has_next(i))
                n -= node->go(i)->data.dp;
        }

        return ret;
    }

    void dump_one(Node *v, FILE *out)
    {
        static ll num = 0;

        if (v->visited)
            return;
        v->visited = true;

        fprintf(out, "struct%p[label=\"[%d](%d)", v, v->idx, v->data.p ? v->data.p->idx : -1);
        // fprintf(out, "cnt = %d\\n", v->data.cnt);
        fprintf(out, "\\nlen = %d", v->len);
        // fprintf(out, "dp = %lld\\n", v->data.dp);
        // if (v->terminal) {
        //     fprintf(out, "\\nTERMINAL");
        // }
        fprintf(out, "\\n");
        std::vector<char> str;
        get_str(v, str);
        for (int i = str.size() - 1; i >= 0; --i) {
            fprintf(out, "%c", str[i]);
        }

        fprintf(out, "\"];\n");
        for (ll i = 'a'; i < 'a' + ALPHABET_SIZE; ++i) {
            if (v->has_next(i - 'a'))
            {
                fprintf(out, "struct%p -> struct%p [label=\"%c\"];\n", v, node(v->next[i - 'a']), (char)i);
                dump_one(node(v->next[i - 'a']), out);
            }
        }
        if (node(v->link))
            fprintf(out, "struct%p -> struct%p [style=\"dashed\"];\n", v, node(v->link));
    }

    void dump()
    {
        FILE *graph = fopen("graph.gv", "w");

        fprintf(graph, "digraph g {\n"
                    "rankdir=\"LR\";\n");
        dump_one(node(root), graph);
        fprintf(graph, "}\n");

        fclose(graph);
        system("dot graph.gv -Tpng -o graph.png");
        system("eog graph.png");
    }

    void get_str(Node *v, std::vector<char> &ans) {
        if (!v || v->idx == root) return;

        ans.push_back(v->c + 'a');
        get_str(v->data.p, ans);
    }

    void print_str(Node *v) {
        std::vector<char> str;
        get_str(v, str);
        // for (int i = str.size() - 1; i >= 0; --i) {
        for (int i = 0; i < str.size(); ++i) {
            printf("%c", str[i]);
        }
        printf("\n");
    }

    void print_tree(Node *v, int n) {
        static int maxn = 0;

        if (!v) return;

        v->data.num = maxn++;

        if (v->idx != root) {
            int rev_r = v->r - 1;
            int rev_l = rev_r - v->len + 1;

            int l = n - rev_r - 1;
            int r = n - rev_l - 1;

            l = r - ((v->len) - node(v->link)->len) + 1;
            r += 1;

            printf("%d %d %d\n", node(v->link)->data.num, l, r);
            // print_str(v);
        }

        // printf("to size %d\n", v->data.to_visit.size());

        for (auto &k : v->data.to_visit) {
            // printf("go %c\n", k.chr);
            print_tree(node(k.idx), n);
        }

        // for (int i = 0; i < v->data.to_visit.size(); ++i) {
        //     if (!node(v->data.to_visit[i])->data.prior) print_tree(node(v->data.to_visit[i]), n);
        // }
    }

};


struct King {
    char chr;
    int idx;
};

bool operator<(const King &one, const King &other) {
        if (one.chr < other.chr) {
            return true;
        } else if (one.chr > other.chr) {
            return false;
        } else {
            return one.idx < other.idx;
        }
    }

struct TaskData {
    SuffixAutomat<char, TaskData>::Node *p = nullptr;
    ll dp = -1;
    ll cnt = 0;
    std::set<King> to_visit;
    int num;
};


using TaskSuffixAutomat = SuffixAutomat<char, TaskData>;


void tolower(char *str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 'a' - 'A';
        }
        ++str;
    }
}


int main() {
    TaskSuffixAutomat automat;

    char *inp = (char*) calloc(100001, sizeof(char));
    scanf("%1000000s", inp);
    int l = strlen(inp);

    std::reverse(inp, inp + l);
    // printf("---\n%s\n", inp);

    inp[0] = 'a' - 1;
    for (int i = 0; i < l; ++i) {
        inp[i] += 1;
    }

    automat.add_string(inp);

    automat.node(automat.root)->data.cnt = 0;
    automat.node(automat.root)->data.dp  = -1;

    std::reverse(inp, inp + l);

    automat.node(automat.root)->task_update(inp, l);

    automat.node(automat.root)->data.cnt = 0;
    automat.node(automat.root)->data.dp  = 0;

    printf("%lu\n", automat.nodes.size());
    automat.print_tree(automat.node(automat.root), l);


    free(inp);

    automat.dump();

    return 0;
}
