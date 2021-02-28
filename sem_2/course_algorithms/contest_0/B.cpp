/*
Пусть все натуральные числа исходно организованы в список в естественном порядке. Разрешается выполнить следующую операцию: swap(a,b). Эта операция возвращает в качестве результата расстояние в текущем списке между числами a и b и меняет их местами.

Задана последовательность операций swap. Требуется вывести в выходной файл результат всех этих операций.
*/

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <algorithm>
 
// hash =======================================================================
 
#define ull unsigned long long
 
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
		if (h == -1) 	{
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
	long long key;
	long long val;
 
	bool operator==(const Pair other) {
		return key == other.key;
	}
};
 
Pair copy(const Pair p) {
	return {p.key, p.val};
}
 
ull hashed(const Pair p) {
	return p.key;
}
 
int main() {
	HashTable<Pair> htable;
 
	int q = 0;
	scanf("%d", &q);
 
	while (q--) {
		long long x, y;
		scanf("%lld %lld", &x, &y);
 
		Pair *p1 = htable.insert({x, x});
		Pair *p2 = htable.insert({y, y});
		printf("%d\n", abs(p1->val - p2->val));
		std::swap(p1->val, p2->val);
	}
 
    return 0;
}
