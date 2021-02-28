/*
Ваша задача — написать программу, моделирующую простое устройство, которое умеет прибавлять целые значения к целочисленным переменным.

Входные данные
Входной файл состоит из одной или нескольких строк, описывающих операции. Строка состоит из названия переменной и числа, которое к этой переменной надо добавить. Все числа не превосходят 100 по абсолютной величине. Изначально все переменные равны нулю. Названия переменных состоят из не более чем 100000 маленьких латинских букв. Размер входного файла не превосходит 2 мегабайта.

Выходные данные
Для каждой операции выведите на отдельной строке значение соответствующей переменной после выполнения операции.


*/


#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
 
// hash =======================================================================
 
#define ull unsigned long long
 
const ull P = 273;
const ull M = 1e9 + 9;
ull *pows = nullptr;
 
ull hashed(char *s) {
    size_t l = strlen(s);
    ull h = s[0];
    for (int i = 1; i < l; ++i) {
        h = (h * P + s[i]) % M;
    }
    return h;
}
 
// list =======================================================================
 
int node_cnt = 0;
 
template <typename T>
class Node {
public:
    T data;
    Node *next;
    Node *prev;
 
    Node(const T val):
    data(copy(val)),
    next(this),
    prev(this)
    {}
 
    void destroy_and_free(bool recursive = true) {
        for (Node<T> *n = next; n && n != this; n = n->next) {
            delete n;
        }
        delete this;
    }
 
    void connect(Node<T> *left, Node<T> *right) {
        if (left) {
            left->next = this;
            this->prev = left;
        }
        if (right) {
            right->prev = this;
            this->next = right;
        }
    }
 
    Node<T> *erase() {
        Node<T> *l = prev;
        Node<T> *r = next;
        if (prev) {
            prev->next = next;
        }
        if (next) {
            next->prev = prev;
        }
        delete this;
 
        if (this == r) {
            return nullptr;
        } else {
            return r;
        }
    }
 
    Node<T> *push_back(const T val) {
        Node<T> *node = new Node(val);
        node->connect(this, next);
        return this;
    }
 
    Node<T> *push_front(const T val) {
        Node<T> *node = new Node(val);
        node->connect(prev, this);
        return node;
    }
};
 
// hash-table =================================================================
 
template <typename T>
class HashTable {
    Node<T> **data;
public:
    int capacity;
 
    HashTable(const int capacity_ = 1000000):
    data((Node<T>**) calloc(capacity_, sizeof(Node<T>**))),
    capacity(capacity_)
    {}
 
    ~HashTable() {
        for (int i = 0; i < capacity; ++i) {
            if (data[i]) {
                data[i]->destroy_and_free();
            }
        }
        free(data);
    }
 
    T *get(const T val, ull h = -1) {
        if (h == -1)    {
            ull h = hashed(val) % capacity;
        }
        if (!data[h]) {
            return nullptr;
        } else {
            Node<T> *head = data[h];
            if (head->data == val) {
                return &head->data;
            }
 
            for (Node<T> *n = head->next; n != head; n = n->next) {
                if (n->data == val) {
                    return &n->data;
                }
            }
        }
        return nullptr;
    }
 
    T *insert(const T val) {
        ull h = hashed(val) % capacity;
        T *stored = get(val, h);
        if (stored) {
            return stored;
        }
 
        if (!data[h]) {
            return &(data[h] = new Node<T>(val))->data;
        } else {
            Node<T> *n = data[h] = data[h]->push_front(val);
            return &n->data;
        }
    }
};
 
// my pair ====================================================================
 
struct Pair {
    char *str;
    long long val;
 
    bool operator==(const Pair &other) {
        return !(strcmp(str, other.str));
    }
};
 
Pair copy(const Pair p) {
    size_t len = strlen(p.str);
    char *cp = (char*) malloc(len + 1);
    strcpy(cp, p.str);
    return {cp, p.val};
}
 
ull hashed(const Pair p) {
    return hashed(p.str);
}
 
int main() {
    HashTable<Pair> htable;
 
    char str[100003] = "";
    long long val;
 
    while(scanf("%s", str) == 1) {
        scanf("%lld", &val);
 
        Pair *p = htable.insert({str, 0});
        p->val += val;
        printf("%lld\n", p->val);
    }
 
    return 0;
}
