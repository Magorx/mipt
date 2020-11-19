#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

// AVL_TREE ===================================================================
template <typename T_KEY>
class AVLTreeNode;

template <typename T_KEY>
int AVLTreeNode_get_balance(const AVLTreeNode<T_KEY> *cake) {
	if (cake == nullptr) {
		return 0;
	} else {
		int r = cake->R == nullptr ? 0 : cake->R->height;
	  	int l = cake->L == nullptr ? 0 : cake->L->height;
	  	return r - l;
	}
}

template <typename T_KEY>
void AVLTreeNode_dump(const AVLTreeNode<T_KEY> *cake, const int depth, const int to_format_cnt) {
	if (!cake) {return;}

	AVLTreeNode_dump(cake->R, depth + 1, to_format_cnt + 1);

	for (int i = 0; i < depth; ++i) {
		printf("    ");
		if (depth - to_format_cnt- 1 <= i) {
			printf("|");
		} else {
			printf(" ");
		}
	}

	printf("%03d>|\n", cake->key);
	AVLTreeNode_dump(cake->L, depth + 1, to_format_cnt + 1);
}

template <typename T_KEY>
class AVLTreeNode {
public:
	T_KEY key;
	AVLTreeNode<T_KEY> *L;
	AVLTreeNode<T_KEY> *R;
	int height;
	int cnt;
	bool multinode;

	AVLTreeNode<T_KEY>() {
		int cnt = 0;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	AVLTreeNode<T_KEY>(const T_KEY &new_key) {
		key = new_key;
		cnt = 1;
		L = nullptr;
		R = nullptr;
		height = 1;
		multinode = 0;
	}

	~AVLTreeNode<T_KEY>() {
		cnt = 0;
		L = nullptr;
		R = nullptr;
		height = 0;
		multinode = 0;
	}

	AVLTreeNode<T_KEY> *&operator[](int i) {
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

	AVLTreeNode<T_KEY> *rotate_left() {
		AVLTreeNode<T_KEY>* node = R;
		R = node->L;
		node->L = this;
		this->update();
		node->update();

		return node;
	}

	AVLTreeNode<T_KEY> *rotate_right() {
		AVLTreeNode<T_KEY>* node = L;
		L = node->R;
		node->R = this;
		this->update();
		node->update();

		return node;
	}

	AVLTreeNode<T_KEY> *rotate_left_big() {
		R = R->rotate_right();
		return this->rotate_left();
	}

	AVLTreeNode<T_KEY> *rotate_right_big() {
		L = L->rotate_left();
		return this->rotate_right();
	}

	AVLTreeNode<T_KEY> *balance() {
		int balance = AVLTreeNode_get_balance(this);
		if (balance == 2) {
			balance = AVLTreeNode_get_balance(R);
			if (balance == 1 || balance == 0) {
				return rotate_left();
			} else {
				return rotate_left_big();
			}
		} else if (balance == -2) {
			balance = AVLTreeNode_get_balance(L);
			if (balance == -1 || balance == 0) {
				return rotate_right();
			} else {
				return 	rotate_right_big();
			}
		} else {
			return this;
		}
	}

	AVLTreeNode<T_KEY> *insert(const T_KEY val) {
		if (key == val) {
			++cnt;
			return this;
		}

		int flag = val > key;
		(*this)[flag] = (*this)[flag] ? (*this)[flag]->insert(val) : new AVLTreeNode(val);
		
		update();
		return balance();
	}

	AVLTreeNode<T_KEY> *dislink(const T_KEY val) {
		if (val == key) {
			return R ? R : L;
		} else {
			int flag = val > key;
			(*this)[flag] = (*this)[flag]->dislink(val);
			update();
			return balance();
		}
	}

	AVLTreeNode<T_KEY> *erase(T_KEY val) {
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
				AVLTreeNode<T_KEY> *l = L;
				delete this;
				return l;
			}

			AVLTreeNode<T_KEY> *r_min = R->min();
			R = R->dislink(r_min->key);
			r_min->R = R;
			r_min->L = L;

			delete this;

			r_min->update();
			return r_min->balance();;
		}
	}

	AVLTreeNode<T_KEY> *min() {
		return L ? L->min() : this;
	}

	AVLTreeNode<T_KEY> *max() {
		return R ? R->max() : this;
	}

	AVLTreeNode<T_KEY> *find(const T_KEY val) {
		if (key == val) {
			return this;
		}

		int flag = key <= val;
		return (*this)[flag] ? (*this)[flag]->find(val) : nullptr;
	}

	AVLTreeNode<T_KEY> *next(const T_KEY val) {
		if (key == val) {
			return R ? R->min() : nullptr;
		} else if (key < val) {
			AVLTreeNode<T_KEY> *ret = R ? R->next(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			AVLTreeNode<T_KEY> *ret = L ? L->next(val) : nullptr;
			return ret ? ret : this;
		}
	}

	AVLTreeNode<T_KEY> *prev(const T_KEY val) {
		if (key == val) {
			return L ? L->max() : nullptr;
		} else if (key > val) {
			AVLTreeNode<T_KEY> *ret = L ? L->prev(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			AVLTreeNode<T_KEY> *ret = R ? R->prev(val) : nullptr;
			return ret ? ret : this;
		}
	}

	void dump(int depth = 0) {
		AVLTreeNode_dump(this, depth, depth);
	}

	void space_dump() {
		if (L) L->space_dump();
		printf("%lld ", key);
		if (R) R->space_dump();
	}
};

template <typename T_KEY>
class AVLTree {
private:
	AVLTreeNode<T_KEY> *root;
	size_t size;

	AVLTreeNode<T_KEY> *find_node(const T_KEY &val) {
		if (!root) {
			return nullptr;
		} else {
			return root->find(val);
		}
	}
public:
	AVLTree<T_KEY>() {
		root = nullptr;
		size = 0;
	}

	~AVLTree<T_KEY>() {
		recursive_node_delete(root);
	}

	void recursive_node_delete(AVLTreeNode<T_KEY> *node) {
		if (!node) {return;}
		recursive_node_delete(node->L);
		recursive_node_delete(node->R);
		delete node;
	}

	void insert(const T_KEY key) {
		if (!root) {
			root = new AVLTreeNode<T_KEY>(key);
			++size;
		} else {
			AVLTreeNode<T_KEY> *node = find_node(key);
			if (!node) {
				root = root->insert(key);
				++size;
			} else {
				node->cnt++;
			}
		}
	}

	void erase(const T_KEY val) {
		if (root) {
			if (root->find(val)) {
				root = root->erase(val);
				--size;
			}
		}
	}

	void clear() {
		recursive_node_delete(root);
		root = nullptr;
		size = 0;
	}

	bool find(const T_KEY val) {
		if (!root) {
			return false;
		}

		if (root->find(val)) {
			return true;
		} else {
			return false;
		}
	}

	T_KEY min() {
		if (root) {
			return root->min()->key;
		} else {
			throw "AVLTree is empty";
		}
	}

	T_KEY max() {
		if (root) {
			return root->max()->key;
		} else {
			throw "AVLTree is empty";
		}
	}

	size_t get_size() {
		return size;
	}

	bool empty() {
		return !size;
	}

	T_KEY *next(const T_KEY val) {
		if (!root) {
			return nullptr;
		}

		AVLTreeNode<T_KEY> *node = root->next(val);
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

		AVLTreeNode<T_KEY> *node = root->prev(val);
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

	void space_dump() {
		if (root) {
			root->space_dump();
		}
	}
};

//=============================================================================
// Мап на этом же дереве :3 ===================================================

template <typename T_KEY, typename T_VAL>
class AVLMapNode;

template <typename T_KEY, typename T_VAL>
int AVLMapNode_get_balance(const AVLMapNode<T_KEY, T_VAL> *cake) {
	if (cake == nullptr) {
		return 0;
	} else {
		int r = cake->R == nullptr ? 0 : cake->R->height;
	  	int l = cake->L == nullptr ? 0 : cake->L->height;
	  	return r - l;
	}
}

template <typename T_KEY, typename T_VAL>
void AVLMapNode_dump(const AVLMapNode<T_KEY, T_VAL> *cake, const int depth, const int to_format_cnt) {
	if (!cake) {return;}

	AVLMapNode_dump(cake->R, depth + 1, to_format_cnt + 1);

	for (int i = 0; i < depth; ++i) {
		printf("    ");
		if (depth - to_format_cnt- 1 <= i) {
			printf("|");
		} else {
			printf(" ");
		}
	}

	printf("%03lld>|\n", cake->key);
	AVLMapNode_dump(cake->L, depth + 1, to_format_cnt + 1);
}

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
		int balance = AVLMapNode_get_balance(this);
		if (balance == 2) {
			balance = AVLMapNode_get_balance(R);
			if (balance == 1 || balance == 0) {
				return rotate_left();
			} else {
				return rotate_left_big();
			}
		} else if (balance == -2) {
			balance = AVLMapNode_get_balance(L);
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
		AVLMapNode_dump(this, depth, depth);
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
			++size;
		} else {
			AVLMapNode<T_KEY, T_VAL> *node = find_node(key);
			if (!node) {
				root = root->insert(key, val);
				++size;
			} else {
				if (node->val == val) {
					node->cnt++;
				} else {
					node->val = val;
					node->cnt = 1;
				}
			}
		}
	}

	bool find(const T_KEY key) {
		AVLMapNode<T_KEY, T_VAL> *node = find_node(key);
		if (node) {
			return true;
		} else {
			return false;
		}
	}

	T_VAL get(const T_KEY key) {
		AVLMapNode<T_KEY, T_VAL> *node = find_node(key);
		if (node) {
			return node->val;
		} else {
			throw "AVLMap doesn't containt key";
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
		if (root) {
			return root->min()->key;
		} else {
			throw "AVLMap is empty";
		}
	}

	T_KEY *max() {
		if (root) {
			return root->max()->key;
		} else {
			throw "AVLMap is empty";
		}
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

int main() {
	AVLMap<long long, char*> map_tree;

	return 0;
}