#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <cassert>





const long long ALPHABET_SIZE = 26;



template <typename KEY_T, typename TASK_DATA> 
class SuffixAutomat {
public:
    struct Node;
    using NodePtr = long long;

private:
    std::vector<Node> nodes;
    NodePtr last;

    Node *new_node(const KEY_T &key = 0, NodePtr prev = -1) {
        nodes.push_back({this, (long long) nodes.size(), key});
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

        long long upper_idx = upper->idx;
        long long cur_idx = cur->idx;
        long long child_idx = child->idx;

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

    struct Node {
        SuffixAutomat *automat;
        NodePtr idx;

        KEY_T c = 0;
        long long len = 0;
        
        NodePtr link = -1;
        NodePtr next[26] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        
        long long l = 0;
        long long r = 0;

        bool terminal = false;
        bool visited = false;

        TASK_DATA data;

        long long task_update() {
            if (data.dp != -1) {
                return data.dp;
            }

            // if (is_leaf()) {
            //     data.dp = 1;
            //     return data.dp;
            // }

            data.dp = 0;
            for (long long c = 0; c < 26; ++c) {
                if (!has_next(c)) continue;

                Node *v = go(c);
                data.dp += v->task_update();

                if (!v->data.p || v->data.p->len < len) {
                    v->data.p = this;
                }

                data.cnt += v->data.cnt;
            }
            data.dp += data.cnt;
            
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

    SuffixAutomat(long long nodes_to_reserve = 100000) :
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
        long long l = strlen(str);

        for (long long i = 0; i < l; ++i) {
            add_key(str[i] - 'a');
        }
        put_terminals();
    }

    bool contatins(const KEY_T *str) {
        long long l = strlen(str);

        Node *cur = node(root);
        for (long long i = 0; i < l; ++i) {
            cur = cur->go(str[i]);
            if (!cur) {
                return false;
            }
        }

        return true;
    }

    void print_max(Node *node) {
        for (long long c = 'z'; c >= 'a'; --c) {
            if (node->has_next(c - 'a')) {
                printf("%c", c);
                print_max(node->go(c - 'a'));
                return;
            }
        }
    }

    bool get_nth(Node *node, long long n, std::vector<char> &ans) {
        if (!node) return false;

        if (n <= node->data.cnt) {
            for (auto x : ans) {
                printf("%c", x);
            }
            printf("\n");
            return true;
        }

        bool ret = false;
        for (long long i = 0; i < 26; ++i)
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
        static long long num = 0;

        if (v->visited)
            return;
        v->visited = true;

        fprintf(out, "struct%p[label=\"[%d](%d)\\n", v, v->idx, v->data.p ? v->data.p->idx : -1);
        fprintf(out, "cnt = %d\\n", v->data.cnt);
        fprintf(out, "len = %d\\n", v->len);
        fprintf(out, "dp = %lld\\n", v->data.dp);
        if (v->terminal) {
            fprintf(out, "\\nTERMINAL");
        }
        fprintf(out, "\"];\n");
        for (long long i = 'a'; i < 'a' + ALPHABET_SIZE; ++i) {
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

};


struct TaskData {
    SuffixAutomat<char, TaskData>::Node *p = nullptr;
    long long dp = -1;
    long long cnt = 0;
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
    long long n = 0;
    scanf("%1000000s %lld", inp, &n);

    automat.add_string(inp);

    automat.node(automat.root)->data.cnt = 0;
    automat.node(automat.root)->data.dp  = -1;

    automat.node(automat.root)->task_update();

    automat.node(automat.root)->data.cnt = 0;
    automat.node(automat.root)->data.dp  = 0;

    free(inp);


    // automat.dump();

    std::vector<char> ans;
    auto node = automat.get_nth(automat.node(automat.root), n, ans);
    if (node) {
        for (auto x : ans) {
            printf("%c", x);
        }
        printf("\n");
        return 0;
    } else {
        printf("No such line.\n");
    }


    // automat.dump();

    return 0;
}
