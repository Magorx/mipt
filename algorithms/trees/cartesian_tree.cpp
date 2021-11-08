#include <cstdlib>
#include <cstdio>
#include <ctime>

long long randlong() {
  return (rand() << 16) ^ rand();
}

template <typename T_KEY>
struct DecaTreeNode {
    T_KEY key;
    long long cnt;
    long long prior;

    DecaTreeNode<T_KEY> *L;
    DecaTreeNode<T_KEY> *R;

    long long de_size; // for implicit keys

    //-task-based

    long long sum;

    //-----------

public:
    DecaTreeNode () {
        prior = randlong();
        L = nullptr;
        R = nullptr;
        de_size = 1;
        sum = 0;
    }

    ~DecaTreeNode() {}

    DecaTreeNode(T_KEY key_) {
        key = key_;
        cnt = 1;
        prior = randlong();
        L = nullptr;
        R = nullptr;
        de_size = 1;

        //-task-based
        sum = key;
        //-----------
    }

    void update() { // update this when children are already up-to-date
        de_size = (L ? L->de_size : 0) + (R ? R->de_size : 0) + 1;
        //-task-based
        sum  = (L ? L->sum : 0)  + (R ? R->sum  : 0) + key * cnt;
        //-----------
    }

    void push() { // push segment operations into children
        //-task-based
        //-----------
    }

    DecaTreeNode<T_KEY> *find(const T_KEY &k) {
        if (key == k) {
            return this;
        } else if (key > k) {
            return L ? L->find(k) : nullptr;
        } else {
            return R ? R->find(k) : nullptr;
        }
    }

    void set_cnt(const T_KEY &k, const long long new_cnt) {
        if (key == k) {
            cnt = new_cnt;
            update();
        } else if (key > k) {
            if (!L) {
                return;
            }

            L->set_cnt(k, new_cnt);
            update();
        } else {
            if (!R) {
                return;
            }

            R->set_cnt(k, new_cnt);
            update();
        }
    }

    DecaTreeNode<T_KEY> *merge(DecaTreeNode<T_KEY> *right) {
        if (!right) {
            return this;
        }

        if (prior > right->prior) {
            push();
            R = R ? R->merge(right) : right;
            update();
            return this;
        } else {
            right->push();
            right->L = this->merge(right->L);
            right->update();
            return right;
        }
    }

    void dump(int depth = 0) {
        if (R) R->dump(depth + 1);
        for (int i = 0; i < depth; ++i) {
            printf("         |");
        }
        printf("%03lld><%03lld>|", key, sum);
        printf("\n");
        if (L) L->dump(depth + 1);
    }

};

template <typename T_KEY>
class DecaTree {
private:
    void split(DecaTreeNode<T_KEY> *node, const T_KEY &key, DecaTreeNode<T_KEY> **L, DecaTreeNode<T_KEY> **R) {
        if (!node) {
            *L = nullptr;
            *R = nullptr;
            return;
        } else {
            node->push();
        }

        DecaTreeNode<T_KEY> *ret_l = nullptr;
        DecaTreeNode<T_KEY> *ret_r = nullptr;

        if (node->key <= key) {
            split(node->R, key, &ret_l, &ret_r);
            node->R = ret_l;
            node->update();
            
            *L = node;
            *R = ret_r;
        } else {
            split(node->L, key, &ret_l, &ret_r);
            node->L = ret_r;
            node->update();

            *L = ret_l;
            *R = node;
        }
    }

public:
    DecaTreeNode<T_KEY> *root;

    DecaTree() {
        root = nullptr;
    }
    ~DecaTree() {
        recursive_node_delete(root);
    }

    void recursive_node_delete(DecaTreeNode<T_KEY> *node) {
        if (node) {
            recursive_node_delete(node->L);
            recursive_node_delete(node->R);
            delete node;
        }
    }

    T_KEY &operator[](const long long i) {
        if (!root) {
            throw "[ERR]<DecaTree>: overflow";
        }
        DecaTreeNode<T_KEY> *node = root->find(i);
        if (!node) {
            throw "[ERR]<DecaTree>: overflow";
        }
        return node->key;
    }

    void set(const long long pos, const T_KEY &val) {
        if (!root || pos >= size() || pos < 0) {
            throw "[ERR]<DecaTree>: overflow";
        }

        root->set(pos, val);
    }

    long long size() {
        if (!root) {
            return 0;
        } else {
            return root->de_size;
        }
    }

    DecaTree<T_KEY> *split(const T_KEY &key) {
        DecaTree<T_KEY> *right_tree = new DecaTree<T_KEY>();
        
        DecaTreeNode<T_KEY> *left  = nullptr;
        DecaTreeNode<T_KEY> *right = nullptr;
        split(root, key, &left, &right);

        right_tree->root = right;
        root = left;

        return right_tree;
    }

    void merge(DecaTree<T_KEY> *right_tree) {
        if (!root) {
            root = right_tree->root;
        } else {
            root = root->merge(right_tree->root);
        }

        right_tree->root = nullptr;
    }

    void insert(const T_KEY &key) {
        if (!root) {
            root = new DecaTreeNode<T_KEY>(key);
            return;
        }

        if (DecaTreeNode<T_KEY>* node = root->find(key)) {
            root->set_cnt(key, node->cnt + 1);
            return;
        }

        DecaTree<T_KEY> *tree_right = split(key);
        push_back(key);
        merge(tree_right);

        delete tree_right;
    }

    void push_back(const T_KEY &val) {
        DecaTreeNode<T_KEY> *node = new DecaTreeNode<T_KEY>(val);
        root = root ? root->merge(node) : node;
    }

    void erase(const long long pos) {
        DecaTree<T_KEY> *tree_mid = split(pos);
        DecaTree<T_KEY> *tree_right = tree_mid->split(1);
        merge(tree_right);

        delete tree_mid;
        delete tree_right;
    }

    void dump() {
        if (root) {
            root->dump();
        }
        printf("\n");
    }

};

int main() {
    DecaTree<long long> tree;

    return 0;
}