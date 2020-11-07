#include <stdexcept>

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

template <typename T_KEY, typename T_VAL>
class AVLMapNode;

template <typename T_KEY, typename T_VAL>
int AVLNode_get_balance(const AVLMapNode<T_KEY, T_VAL> *cake) {
	if (cake == nullptr) {
		return 0;
	} else {
		int r = cake->R == nullptr ? 0 : cake->R->height;
	  	int l = cake->L == nullptr ? 0 : cake->L->height;
	  	return r - l;
	}
}

template <typename T_KEY, typename T_VAL>
void AVLNode_dump(const AVLMapNode<T_KEY, T_VAL> *cake, const int depth, const int to_format_cnt) {
	if (!cake) {return;}

	AVLNode_dump(cake->R, depth + 1, to_format_cnt + 1);

	for (int i = 0; i < depth; ++i) {
		printf("    ");
		if (depth - to_format_cnt- 1 <= i) {
			printf("|");
		} else {
			printf(" ");
		}
	}

	printf("%03d>|\n", cake->key);
	AVLNode_dump(cake->L, depth + 1, to_format_cnt + 1);
}

// Дерево из A ================================================================
template <typename T>
class AVLNode {
public:
	T key;
	AVLNode<T> *L;
	AVLNode<T> *R;
	int height;
	int cnt;
	bool multinode;

	AVLNode<T>() {
		key = 0;
		int cnt = 0;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	AVLNode<T>(const T &new_key) {
		key = new_key;
		cnt = 1;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	~AVLNode<T>() {
		key = 0;
		cnt = 0;
		L = nullptr;
		R = nullptr;
		height = 0;
		multinode = 0;
	}

	AVLNode<T> *&operator[](int i) {
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

	AVLNode<T> *rotate_left() {
		AVLNode<T>* node = R;
		R = node->L;
		node->L = this;
		this->update();
		node->update();

		return node;
	}

	AVLNode<T> *rotate_right() {
		AVLNode<T>* node = L;
		L = node->R;
		node->R = this;
		this->update();
		node->update();

		return node;
	}

	AVLNode<T> *rotate_left_big() {
		R = R->rotate_right();
		return this->rotate_left();
	}

	AVLNode<T> *rotate_right_big() {
		L = L->rotate_left();
		return this->rotate_right();
	}

	AVLNode<T> *balance() {
		int balance = AVLNode_get_balance(this);
		if (balance == 2) {
			balance = AVLNode_get_balance(R);
			if (balance == 1 || balance == 0) {
				return rotate_left();
			} else {
				return rotate_left_big();
			}
		} else if (balance == -2) {
			balance = AVLNode_get_balance(L);
			if (balance == -1 || balance == 0) {
				return rotate_right();
			} else {
				return 	rotate_right_big();
			}
		} else {
			return this;
		}
	}

	AVLNode<T> *insert(const T val) {
		if (key == val) {
			++cnt;
			return this;
		}

		int flag = val > key;
		(*this)[flag] = (*this)[flag] ? (*this)[flag]->insert(val) : new AVLNode(val);
		
		update();
		return balance();
	}

	AVLNode<T> *dislink(const T val) {
		if (val == key) {
			return R ? R : L;
		} else {
			int flag = val > key;
			(*this)[flag] = (*this)[flag]->dislink(val);
			update();
			return balance();
		}
	}

	AVLNode<T> *erase(T val) {
		if (val < key) {
			if (L) {
				L = L->erase(val);
				update();
				return balance();
			} else {
				return this;
			}
		} else if (val > key) {
			if (R) {
				R = R->erase(val);
				update();
				return balance();
			} else {
				return this;
			}
		} else {
			if (multinode) {
				--cnt;
				if (cnt > 0) {
					return this;
				}
			}

			if (!R) {
				AVLNode<T> *l = L;
				delete this;
				return l;
			}

			AVLNode<T> *r_min = R->min();
			R = R->dislink(r_min->key);
			r_min->R = R;
			r_min->L = L;

			delete this;

			r_min->update();
			return r_min->balance();;
		}
	}

	AVLNode<T> *min() {
		return L ? L->min() : this;
	}

	AVLNode<T> *max() {
		return R ? R->max() : this;
	}

	AVLNode<T> *find(const T val) {
		if (key == val) {
			return this;
		}

		int flag = key <= val;
		return (*this)[flag] ? (*this)[flag]->find(val) : nullptr;
	}

	AVLNode<T> *next(const T val) {
		if (key == val) {
			return R ? R->min() : nullptr;
		} else if (key < val) {
			AVLNode<T> *ret = R ? R->next(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			AVLNode<T> *ret = L ? L->next(val) : nullptr;
			return ret ? ret : this;
		}
	}

	AVLNode<T> *prev(const T val) {
		if (key == val) {
			return L ? L->max() : nullptr;
		} else if (key > val) {
			AVLNode<T> *ret = L ? L->prev(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			AVLNode<T> *ret = R ? R->prev(val) : nullptr;
			return ret ? ret : this;
		}
	}

	void dump(int depth = 0) {
		AVLNode_dump(this, depth, depth);
	}
};

template <typename T>
class AVLTree {
private:
	AVLNode<T> *root;
	size_t size;
public:
	AVLTree<T>() {
		root = nullptr;
		size = 0;
	}

	~AVLTree<T>() {
		recursive_node_delete(root);
	}

	void recursive_node_delete(AVLNode<T> *node) {
		if (!node) {return;}
		recursive_node_delete(node->L);
		recursive_node_delete(node->R);
		delete node;
	}

	void insert(const T val) {
		if (!root) {
			root = new AVLNode<T>(val);
		} else {
			root = root->insert(val);
		}
		++size;
	}

	void erase(const T val) {
		if (root) {
			if (root->find(val)) {
				root = root->erase(val);
			}
		}
		--size;
	}

	void clear() {
		recursive_node_delete(root);
		root = nullptr;
		size = 0;
	}

	T *find(const T val) {
		if (!root) {
			return nullptr;
		}

		AVLNode<T> *ret = root->find(val);
		return ret ? &ret->key : nullptr;
	}

	T *min() {
		return root ? root->min()->key : (T) 0;
	}

	T *max() {
		return root ? root->max()->key : (T) 0;
	}

	T *next(const T val) {
		if (!root) {
			return nullptr;
		}

		AVLNode<T> *node = root->next(val);
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

		AVLNode<T> *node = root->prev(val);
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

//=============================================================================
// Мап на этом же дереве :3 ===================================================

template <typename T_KEY, typename T_VAL>
class AVLMapNode {
public:
	T_KEY key;
	T_VAL val;
	AVLMapNode<T_KEY, T_VAL> *L;
	AVLMapNode<T_KEY, T_VAL> *R;
	int height;
	int cnt;
	bool multinode;

	AVLMapNode<T_KEY, T_VAL>() {
		int cnt = 0;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	AVLMapNode<T_KEY, T_VAL>(const T_KEY &new_key, const T_VAL &new_val) {
		key = new_key;
		val = new_val;
		cnt = 1;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	~AVLMapNode<T_KEY, T_VAL>() {
		cnt = 0;
		L = nullptr;
		R = nullptr;
		height = 0;
		multinode = 0;
	}

	AVLMapNode<T_KEY, T_VAL> *&operator[](int i) {
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

	AVLMapNode<T_KEY, T_VAL> *rotate_left() {
		AVLMapNode<T_KEY, T_VAL>* node = R;
		R = node->L;
		node->L = this;
		this->update();
		node->update();

		return node;
	}

	AVLMapNode<T_KEY, T_VAL> *rotate_right() {
		AVLMapNode<T_KEY, T_VAL>* node = L;
		L = node->R;
		node->R = this;
		this->update();
		node->update();

		return node;
	}

	AVLMapNode<T_KEY, T_VAL> *rotate_left_big() {
		R = R->rotate_right();
		return this->rotate_left();
	}

	AVLMapNode<T_KEY, T_VAL> *rotate_right_big() {
		L = L->rotate_left();
		return this->rotate_right();
	}

	AVLMapNode<T_KEY, T_VAL> *balance() {
		int balance = AVLNode_get_balance(this);
		if (balance == 2) {
			balance = AVLNode_get_balance(R);
			if (balance == 1 || balance == 0) {
				return rotate_left();
			} else {
				return rotate_left_big();
			}
		} else if (balance == -2) {
			balance = AVLNode_get_balance(L);
			if (balance == -1 || balance == 0) {
				return rotate_right();
			} else {
				return 	rotate_right_big();
			}
		} else {
			return this;
		}
	}

	AVLMapNode<T_KEY, T_VAL> *insert(const T_KEY &new_key, const T_VAL &new_val) {
		if (key == new_key) {
			++cnt;
			return this;
		}

		int flag = new_key > key;
		(*this)[flag] = (*this)[flag] ? (*this)[flag]->insert(new_key, new_val) : new AVLMapNode(new_key, new_val);
		
		update();
		return balance();
	}

	AVLMapNode<T_KEY, T_VAL> *dislink(const T_KEY dislink_key) {
		if (dislink_key == key) {
			return R ? R : L;
		} else {
			int flag = dislink_key > key;
			(*this)[flag] = (*this)[flag]->dislink(dislink_key);
			update();
			return balance();
		}
	}

	AVLMapNode<T_KEY, T_VAL> *erase(T_KEY remove_key) {
		if (remove_key < key) {
			if (L) {
				L = L->erase(remove_key);
				update();
				return balance();
			} else {
				return this;
			}
		} else if (remove_key > key) {
			if (R) {
				R = R->erase(remove_key);
				update();
				return balance();
			} else {
				return this;
			}
		} else {
			if (multinode) {
				--cnt;
				if (cnt > 0) {
					return this;
				}
			}

			if (!R) {
				AVLMapNode<T_KEY, T_VAL> *l = L;
				delete this;
				return l;
			}

			AVLMapNode<T_KEY, T_VAL> *r_min = R->min();
			R = R->dislink(r_min->key);
			r_min->R = R;
			r_min->L = L;

			delete this;

			r_min->update();
			return r_min->balance();;
		}
	}

	AVLMapNode<T_KEY, T_VAL> *min() {
		return L ? L->min() : this;
	}

	AVLMapNode<T_KEY, T_VAL> *max() {
		return R ? R->max() : this;
	}

	AVLMapNode<T_KEY, T_VAL> *find(const T_KEY val) {
		if (key == val) {
			return this;
		}

		int flag = key <= val;
		return (*this)[flag] ? (*this)[flag]->find(val) : nullptr;
	}

	AVLMapNode<T_KEY, T_VAL> *next(const T_KEY val) {
		if (key == val) {
			return R ? R->min() : nullptr;
		} else if (key < val) {
			AVLMapNode<T_KEY, T_VAL> *ret = R ? R->next(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			AVLMapNode<T_KEY, T_VAL> *ret = L ? L->next(val) : nullptr;
			return ret ? ret : this;
		}
	}

	AVLMapNode<T_KEY, T_VAL> *prev(const T_KEY val) {
		if (key == val) {
			return L ? L->max() : nullptr;
		} else if (key > val) {
			AVLMapNode<T_KEY, T_VAL> *ret = L ? L->prev(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			AVLMapNode<T_KEY, T_VAL> *ret = R ? R->prev(val) : nullptr;
			return ret ? ret : this;
		}
	}

	void dump(int depth = 0) {
		AVLNode_dump(this, depth, depth);
	}
};

template <typename T_KEY, typename T_VAL>
class AVLMap {
private:
	AVLMapNode<T_KEY, T_VAL> *root;
	size_t size;

	AVLMapNode<T_KEY, T_VAL> *find_node(const T_KEY key) {
		if (!root) {
			return nullptr;
		}

		return root->find(key);
	}

public:
	AVLMap<T_KEY, T_VAL>() {
		root = nullptr;
		size = 0;
	}

	~AVLMap<T_KEY, T_VAL>() {
		recursive_node_delete(root);
	}

	void recursive_node_delete(AVLMapNode<T_KEY, T_VAL> *node) {
		if (!node) {return;}
		recursive_node_delete(node->L);
		recursive_node_delete(node->R);
		delete node;
	}

	void insert(const T_KEY key, const T_VAL val) {
		if (!root) {
			root = new AVLMapNode<T_KEY, T_VAL>(key, val);
		} else {
			AVLMapNode<T_KEY, T_VAL> *node = find_node(key);
			if (!node) {
				root = root->insert(key, val);
			} else {
				if (node->val == val) {
					node->cnt++;
				} else {
					node->val = val;
					node->cnt = 1;
				}
			}
		}
		++size;
	}

	T_VAL get(const T_KEY key) {
		AVLMapNode<T_KEY, T_VAL> *node = find_node(key);
		if (node) {
			return node->val;
		} else {
			throw std::invalid_argument("AVLMap doesn't containt key");
		}
	}

	void erase(const T_KEY val) {
		if (root) {
			if (root->find(val)) {
				root = root->erase(val);
			}
		}
		--size;
	}

	void clear() {
		recursive_node_delete(root);
		root = nullptr;
		size = 0;
	}

	T_KEY *min() {
		return root ? root->min()->key : (T_KEY) 0;
	}

	T_KEY *max() {
		return root ? root->max()->key : (T_KEY) 0;
	}

	T_KEY *next(const T_KEY val) {
		if (!root) {
			return nullptr;
		}

		AVLMapNode<T_KEY, T_VAL> *node = root->next(val);
		if (!node) {
			return nullptr;
		} else {
			return &node->key;
		}
	}

	T_KEY *prev(const T_KEY val) {
		if (!root) {
			return nullptr;
		}

		AVLMapNode<T_KEY, T_VAL> *node = root->prev(val);
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

//=============================================================================
template <typename T>
void avl_node_dump(AVLNode<T> *n) {
	if (!n) {return;}

	avl_node_dump(n->L);
	printf("%lld ", n->key);
	avl_node_dump(n->R);
}


template <typename T>
void avl_task_dump(AVLTree<T> *t) {
	if 
}

int main() {
	long long n, m, k;
	scanf("%lld %lld %lld", &n, &m, &k);
	++m;
	AVLTree<AVLTree<long long>*> to_delete;
	AVLMap<long long, AVLTree<long long>*> num_to_set;    // [n] -> [sets containing n]
	AVLTree<long long> *sets = new AVLTree<long long>[m]; // sets[i] - set of index i

	char str[20] = {};
    while (scanf("%15s", str) == 1) {
        if (       strcmp(str, "ADD") == 0 || str[0] == 'a') {
        	long long x = 0;
        	long long s = 0;
        	scanf("%lld %lld\n", &x, &s);

            sets[s].insert(x);
            num_to_set->insert(x, &sets[s]);
        } else if (strcmp(str, "DELETE" || str[0] == 'd') == 0) {
        	long long x = 0;
        	long long s = 0;
        	scanf("%lld %lld\n", &x, &s);

            sets[s].erase(x);
            num_to_set->get(x)->erase()
        } else if (strcmp(str, "CLEAR") == 0) {
        	long long s = 0;
        	scanf("%lld\n", &s);

        	sets[s].clear();          
        } else if (strcmp(str, "LISTSET") == 0) {
        	long long s = 0;
        	scanf("%lld\n", &s);
        } else if(strcmp(str, "LISTSETOF") == 0) {
        	int *ret = tree.prev(x);
        	if (ret) {
        		printf("%d\n", *ret);
        	} else {
        		printf("none\n");
        	}
        } else if (str[0] == 'd') {
            
        }
        //tree.dump();
    }*/

	delete[] sets;

	return 0;
}

// AVL-дерево даровало нам свой логарифм для O(nlogn)