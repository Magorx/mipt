#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <cassert>



const int ALPHABET_SIZE = 37;


template <typename KEY_T, typename TASK_DATA> 
class SuffixAutomat {
public:
    struct Node;
    using NodePtr = int;

private:
    Node *new_node(const KEY_T &key = 0, NodePtr prev = -1) {
        nodes.push_back({this, (int) nodes.size(), key});
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

        int upper_idx = upper->idx;
        int cur_idx = cur->idx;
        int child_idx = child->idx;

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
        int len = 0;
        
        NodePtr link = -1;
        NodePtr next[ALPHABET_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        
        int l = 0;
        int r = 0;

        bool terminal = false;
        bool visited = false;

        TASK_DATA data;

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

        void upd_reach(Node *other = nullptr) {
            if (other) {
                for (int i = 0; i < 10; ++i) {
                    data.reach[i] = data.reach[i] || other->data.reach[i] || (c + 'a') == ('z' + 1 + i);
                }
            }

            int idx = -1;
            for (int i = 0; i < 10; ++i) {
                if ((c + 'a') == ('z' + 1 + i)) {
                    idx = i;
                    break;
                }
            }
            if (idx >= 0) {
                for (int i = 0; i < 10; ++i) {
                    data.reach[i] = false;
                }
                data.reach[idx] = true;
            }
        }

        void task_update() {
            if (data.p != nullptr) {
                return;
            }

            for (int c = 0; c < ALPHABET_SIZE; ++c) {
                if (!has_next(c)) continue;
                Node *node = go(c);
                node->task_update();

                if (!node->data.p || node->data.p->len < len) {
                    node->data.p = this;
                }

                upd_reach(node);
            }
            upd_reach();
        }
    };


public:
    std::vector<Node> nodes;
    NodePtr last;
    NodePtr root;

    SuffixAutomat(int nodes_to_reserve = 1000000) :
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

    void add_string(const KEY_T *str) {
        int l = strlen(str);
        for (int i = 0; i < l; ++i) {
            add_key(str[i] - 'a');
        }
    }

    bool contatins(const KEY_T *str) {
        int l = strlen(str);

        Node *cur = node(root);
        for (int i = 0; i < l; ++i) {
            cur = cur->go(str[i]);
            if (!cur) {
                return false;
            }
        }

        return true;
    }

    const char *byte_to_binary
    (
        int x
    )
    {
        static char b[9];
        b[0] = '\0';

        int z;
        for (z = 128; z > 0; z >>= 1)
        {
            strcat(b, ((x & z) == z) ? "1" : "0");
        }

        return b;
    }

    void dump_one(Node *v, FILE *out)
    {
        static int num = 0;

        if (v->visited)
            return;
        v->visited = true;

        fprintf(out, "struct%p[label=\"[%d](%d)\\n", v, v->idx, v->data.p ? v->data.p->idx : -1);
        for (int i = 0; i < 10; ++i) {
            fprintf(out, "%d", v->data.reach[i] ? 1 : 0);
        }
        // fprintf(out, ")");
        if (v->c + 'a' > 'z') {
            fprintf(out, "\\n%c", '0' + v->c - 25);
        }
        fprintf(out, "\"];\n");
        for (int i = 'a'; i <= 'a' + 28; ++i) {
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
    bool reach[10];
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

bool full_reachable(TaskSuffixAutomat::Node *node, int n) {
    for (int i = 0; i < n; ++i) {
        if (!node->data.reach[i]) {
            return false;
        }
    }

    return true;
}


int main() {
    TaskSuffixAutomat automat;

    char *inp = (char*) calloc(1000000, sizeof(char));
    int n = 0;
    char *s = inp;
    
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        char t;
        scanf("%s%c", s, &t);
        s += strlen(s);
        *s = 'z' + 1 + i;
        ++s;
    }
    // printf("%s\n", inp);

    automat.add_string(inp);
    automat.node(automat.root)->task_update();

    int maxlen = 0;
    TaskSuffixAutomat::Node *maxnode = nullptr;

    for (int i = 0; i < automat.nodes.size(); ++i) {
        auto v = automat.node(i);
        if (!full_reachable(v, n)) continue;

        if (v->len > maxlen) {
            maxlen = v->len;
            maxnode = v;
        }
    }

    std::vector<char> ans;
    automat.get_str(maxnode, ans);
    
    for (int i = ans.size() - 1; i >= 0; --i) {
        printf("%c", ans[i]);
    }
    printf("\n");

    // automat.dump();

    return 0;
}
