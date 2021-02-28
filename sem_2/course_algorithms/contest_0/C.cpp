/*
Енот Вася — начинающий художник. Недавно он приобрёл подержанную кисточку (как рассказал продавец, она сделана из хвоста самого Малевича!), достал холст и решил произвести на свет свой первый шедевр.

Как оказалось, лет кисточке немало, потому она способна лишь ставить кляксы в форме пяти квадратиков, расположенных крестиком (координаты их центров будут равны (x,y), (x−1,y), (x,y−1), (x+1,y), (x,y+1)). Вася поставил N клякс, разочаровался в идее первого шедевра и задумался о месте для нового. Но ведь если он закрасил весь холст, писать будет негде...

Выясните, закрасил ли своими действиями Вася весь холст.
*/

#include <cstdlib>
#include <cstdio>
#include <cctype>
 
// hash =======================================================================
 
#define ull unsigned long long
const ull P = 273;
const ull M = 1e9 + 9;
 
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
 
	HashTable(const int capacity_ = 14000000):
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
 
		++cur_size;
 
		if (!data[h]) {
			return &(data[h] = new Node<T>(val))->data;
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
	long long x;
	long long y;
 
	bool operator==(const Pair other) {
		return x == other.x && y == other.y;
	}
};
 
Pair copy(const Pair p) {
	return {p.x, p.y};
}
 
ull hashed(const Pair p) {
	return (107 + (2971215073 * p.x + 2971215073 * p.y) % M + p.y * p.x * 2971215073);
}
 
bool check_xy(const int w, const int h, const int x, const int y) {
	return x >= 1 && x <= w && y >= 1 && y <= h;
}
 
unsigned long long read_next_long_long(char **buffer) {
    char *c = *buffer;
    while (c && isspace(*c)) ++c;
 
    unsigned long long l = 0;
    while (*c >= '0' && *c <= '9') {
        l = l * 10 + *c - '0';
        ++c;
    }
 
    *buffer = c;
    return l;
}
 
int main() {
	FILE *fin = stdin;
 
    fseek(fin, 0, SEEK_END);
    long input_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);
 
    char *buffer = (char*) calloc(input_size + 1, sizeof(char));
    char *input_init_pointer_for_free = buffer;
    fread(buffer, sizeof(char), input_size, fin);
    fclose(fin);
//---------------------------------------------------------------
	HashTable<Pair> htable;
 
	long long w, h, n;
	w = read_next_long_long(&buffer);
	h = read_next_long_long(&buffer);
	n = read_next_long_long(&buffer);
	// scanf("%lld %lld %lld", &w, &h, &n);
 
	while (n--) {
		long long x, y;
		x = read_next_long_long(&buffer);
		y = read_next_long_long(&buffer);
		// scanf("%lld %lld", &x, &y);
 
		if (x > 1 && y > 1 && x < w && y < h) {
			htable.insert({x    , y    });
			htable.insert({x + 1, y    });
			htable.insert({x - 1, y    });
			htable.insert({x    , y + 1});
			htable.insert({x    , y - 1});
		} else if (x < 0 || y < 0 || x > w + 1 || y > h + 1) {
			continue;
		} else {
			if (check_xy(w, h, x    , y    )) htable.insert({x    , y    });
			if (check_xy(w, h, x + 1, y    )) htable.insert({x + 1, y    });
			if (check_xy(w, h, x - 1, y    )) htable.insert({x - 1, y    });
			if (check_xy(w, h, x    , y + 1)) htable.insert({x    , y + 1});
			if (check_xy(w, h, x    , y - 1)) htable.insert({x    , y - 1});
		}
	}
 
	printf("%s\n", htable.size() == w * h ? "Yes" : "No");
 
    return 0;
}
