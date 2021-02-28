/*
Реализуйте ассоциативный массив с использованием хеш-таблицы. Использовать стандартную библиотеку (set, map, LinkedHashMap, и т. п.) не разрешается.

Входные данные
Входной файл содержит описание операций, их количество не превышает 100000. В каждой строке находится одна из следующих операций:

put x y — поставить в соответствие ключу x значение y. Если ключ уже есть, то значение необходимо изменить.
delete x — удалить ключ x. Если элемента x нет, то ничего делать не надо.
get x — если ключ x есть в ассоциативном массиве, то выведите соответствующее ему значение, иначе выведите «none».
*/

#include <cstdlib>
#include <cstdio>
#include <cctype>
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
	int cur_size;
 
	HashTable(const int capacity_ = 1000000):
	data((Node<T>**) calloc(capacity_, sizeof(Node<T>**))),
	capacity(capacity_),
	cur_size(0)
	{}
 
	~HashTable() {
		// for (int i = 0; i < capacity; ++i) {
		// 	if (data[i]) {
		// 		data[i]->destroy_and_free();
		// 	}
		// }
		free(data);
	}
 
	T *get(const T val, ull h = 0) {
		if (h == 0) {
			h = hashed(val) % capacity;
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
 
		++cur_size;
 
		if (!data[h]) {
			data[h] = new Node<T>(val);
			return &data[h]->data;
		} else {
			Node<T> *n = data[h] = data[h]->push_front(val);
			return &n->data;
		}
	}
 
	int size() {
		return cur_size;
	}
};
 
// my pair ====================================================================
 
struct Pair {
	char *key;
	char *val;
 
	bool operator==(const Pair &other) {
		return !(strcmp(key, other.key));
	}
};
 
char *copy(const char *str) {
	size_t len = strlen(str);
	char *cp = (char*) calloc(len + 1, sizeof(char));
	strcpy(cp, str);
	return cp;
}
 
Pair copy(const Pair p) {
	return {copy(p.key), copy(p.val)};
}
 
ull hashed(const Pair p) {
	return hashed(p.key);
}
 
int main() {
	FILE *fin  = fopen("map.in", "r");
	FILE *fout = fopen("map.out", "w");
 
	HashTable<Pair> htable;
 
	char opr[10] = "";
	char key[25] = "";
	char val[25] = "";
 
	while (fscanf(fin, "%s", opr) == 1) {
		if (opr[0] == 'p') {
			fscanf(fin, "%s %s", key, val);
 
			Pair *p = htable.insert({key, val});
			free(p->val);
			p->val = copy(val);
		} else if (opr[0] == 'd') {
			fscanf(fin, "%s", key);
 
			Pair *p = htable.insert({key, val});
			free(p->val);
			p->val = copy("none");
		} else {
			fscanf(fin, "%s", key);
 
			Pair *p = htable.get({key, val});
			if (!p) {
				fprintf(fout, "none\n");
			} else {
				fprintf(fout, "%s\n", p->val);
			}
		}
	}
 
	fclose(fin);
	fclose(fout);
 
    return 0;
}
