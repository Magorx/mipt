#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

template <typename T>
class Node;

template <typename T>
int Node_get_balance(const Node<T> *cake) {
	if (cake == nullptr) {
		return 0;
	} else {
		int r = cake->R == nullptr ? 0 : cake->R->height;
	  	int l = cake->L == nullptr ? 0 : cake->L->height;
	  	return r - l;
	}
}

template <typename T>
void Node_dump(const Node<T> *cake, const int depth, const int to_format_cnt) {
	if (!cake) {return;}

	Node_dump(cake->R, depth + 1, to_format_cnt + 1);

	for (int i = 0; i < depth; ++i) {
		printf("    ");
		if (depth - to_format_cnt- 1 <= i) {
			printf("|");
		} else {
			printf(" ");
		}
	}

	printf("%03d>|\n", cake->key);
	Node_dump(cake->L, depth + 1, to_format_cnt + 1);
}

template <typename T>
class Node {
public:
	T key;
	Node<T> *L;
	Node<T> *R;
	int height;
	int cnt;
	bool multinode;

	Node<T>() {
		key = 0;
		int cnt = 0;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	Node<T>(const T &new_key) {
		key = new_key;
		cnt = 1;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	Node<T> *&operator[](int i) {
		switch(i % 2) {
			case 0:
				return L;
			case 1:
				return R;
			default:
				printf("wtf man...\n");
				return R;
		}
	}

	void update() {
		int r = R ? R->height : 0;
  		int l = L ? L->height : 0;
  		height = (r > l ? r : l) + 1;
	}

	Node<T> *rotate_left() {
		Node<T>* node = R;
		R = node->L;
		node->L = this;
		this->update();
		node->update();

		return node;
	}

	Node<T> *rotate_right() {
		Node<T>* node = L;
		L = node->R;
		node->R = this;
		this->update();
		node->update();

		return node;
	}

	Node<T> *rotate_left_big() {
		R = R->rotate_right();
		return this->rotate_left();
	}

	Node<T> *rotate_right_big() {
		L = L->rotate_left();
		return this->rotate_right();
	}

	Node<T> *balance() {
		int balance = Node_get_balance(this);
		if (balance == 2) {
			balance = Node_get_balance(R);
			if (balance == 1 || balance == 0) {
				return rotate_left();
			} else {
				return rotate_left_big();
			}
		} else if (balance == -2) {
			balance = Node_get_balance(L);
			if (balance == -1 || balance == 0) {
				return rotate_right();
			} else {
				return 	rotate_right_big();
			}
		} else {
			return this;
		}
	}

	Node<T> *insert(const T val) {
		if (key == val) {
			++cnt;
			return this;
		}

		int flag = val > key;
		(*this)[flag] = (*this)[flag] ? (*this)[flag]->insert(val) : new Node(val);
		
		update();
		return balance();
	}

	Node<T> *dislink(const T val) {
		if (val == key) {
			return R ? R : L;
		} else {
			int flag = val > key;
			(*this)[flag] = (*this)[flag]->dislink(val);
			update();
			return balance();
		}
	}

	Node<T> *erase(T val) {
		if (val < key) {
			assert(L);
			L = L->erase(val);
			update();
			return balance();
		} else if (val > key) {
			assert(R);
			R = R->erase(val);
			update();
			return balance();
		} else {
			if (multinode) {
				--cnt;
				if (cnt > 0) {
					return this;
				}
			}

			if (!R && !L) {
				delete this;
				return nullptr;
			}
			if (!R) {
				L = nullptr;
				delete this;
				return L;
			}

			Node<T> *r_min = R->min();
			R = R->dislink(r_min->key);
			r_min->R = R;
			r_min->L = L;

			R = nullptr;
			L = nullptr;
			delete this;

			r_min->update();
			return r_min->balance();;
		}
	}

	Node<T> *min() {
		return L ? L->min() : this;
	}

	Node<T> *max() {
		return R ? R->max() : this;
	}

	Node<T> *find(const T val) {
		if (key == val) {
			return this;
		}

		int flag = key <= val;
		return (*this)[flag] ? (*this)[flag]->find(val) : nullptr;
	}

	Node<T> *next(const T val) {
		if (key == val) {
			return R ? R->min() : nullptr;
		} else if (key < val) {
			Node<T> *ret = R ? R->next(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			Node<T> *ret = L ? L->next(val) : nullptr;
			return ret ? ret : this;
		}
	}

	Node<T> *prev(const T val) {
		if (key == val) {
			return L ? L->max() : nullptr;
		} else if (key > val) {
			Node<T> *ret = L ? L->prev(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			Node<T> *ret = R ? R->prev(val) : nullptr;
			return ret ? ret : this;
		}
	}

	void dump(int depth = 0) {
		Node_dump(this, depth, depth);
	}
};

template <typename T>
class AVLtree {
private:
	Node<T> *root;
	size_t size;
public:
	AVLtree<T>() {
		root = nullptr;
		size = 0;
	}

	~AVLtree<T>() {
		//recursive_node_delete(root);
	}

	void recursive_node_delete(Node<T> *node) {
		if (!node) {return;}
		if (node->L) {recursive_node_delete(node->L);}
		if (node->R) {recursive_node_delete(node->R);}
		delete node;
	}

	void insert(const T val) {
		if (!root) {
			root = new Node<T>(val);
		} else {
			root = root->insert(val);
		}
	}

	void erase(const T val) {
		if (root) {
			if (root->find(val)) {
				root = root->erase(val);
			}
		}
	}

	T *find(const T val) {
		if (!root) {
			return nullptr;
		}

		Node<T> *ret = root->find(val);
		return ret ? &ret->key : nullptr;
	}

	T *next(const T val) {
		if (!root) {
			return nullptr;
		}

		Node<T> *node = root->next(val);
		if (!node) {
			return nullptr;
		} else {
			return &node->key;
		}
	}

	T *prev(const T val) {
		if (!root) {
			return nullptr;
		}

		Node<T> *node = root->prev(val);
		if (!node) {
			return nullptr;
		} else {
			return &node->key;
		}
	}

	void dump() {
		if (root) {
			root->dump();
		}
	}
};

int main() {
	AVLtree<int> tree;

	char str[10] = {};
    while (scanf("%6s", str) == 1) {
        int x;
        scanf("%d", &x);
        if (       str[0] == 'i') {
            tree.insert(x);
        } else if (str[0] == 'd') {
            tree.erase(x);
        } else if (str[0] == 'e') {
        	printf("%s\n", tree.find(x) == nullptr ? "false" : "true");            
        } else if (str[0] == 'n') {
        	int *ret = tree.next(x);
        	if (ret) {
        		printf("%d\n", *ret);
        	} else {
        		printf("none\n");
        	}
        } else if(str[0] == 'p') {
        	int *ret = tree.prev(x);
        	if (ret) {
        		printf("%d\n", *ret);
        	} else {
        		printf("none\n");
        	}
        } else if (str[0] == 'd') {
            
        }
        //tree.dump();
    }

	//printf("%lld %lld\n", (*a)[0], (*a)[1]);

	return 0;
}