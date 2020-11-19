#include <cstdlib>
#include <cstdio>

template <typename T_KEY>
class SplayTreeNode;


template <typename T_KEY>
void SplayTreeNode_dump(const SplayTreeNode<T_KEY> *cake, const int depth, const int to_format_cnt) {
	if (!cake) {return;}
	if (depth > 10) {return;}

	SplayTreeNode_dump(cake->R, depth + 1, to_format_cnt + 1);

	for (int i = 0; i < depth; ++i) {
		printf("    ");
		if (depth - to_format_cnt- 1 <= i) {
			printf("|");
		} else {
			printf(" ");
		}
	}

	printf("%03lld>|\n", cake->key);
	SplayTreeNode_dump(cake->L, depth + 1, to_format_cnt + 1);
}


template <typename T_KEY>
class SplayTreeNode {
public:
	static const int LEFT  = 0;
	static const int RIGHT = 1;
	static const int SMALL = 0;
	static const int BIG   = 1;

	T_KEY key;
	SplayTreeNode<T_KEY> *L;
	SplayTreeNode<T_KEY> *R;
	
	SplayTreeNode<T_KEY> *P;
	int side;

	int height;
	int cnt;
	bool multinode;

	// task-based =====
	long long s;
	//=================
	SplayTreeNode<T_KEY>() {
		int cnt = 0;
		L = nullptr;
		R = nullptr;
		P = nullptr;
		side = 0;
		height = 1;
		multinode = 0;

		// task-based =====
		s = 0;
		//=================
	}

	SplayTreeNode<T_KEY>(const T_KEY &new_key) {
		key = new_key;
		cnt = 1;
		L = nullptr;
		R = nullptr;
		P = nullptr;
		side = 0;
		height = 1;
		multinode = 0;

		// task-based =====
		s = new_key;
		//=================
	}

	~SplayTreeNode<T_KEY>() {
		cnt = 0;
		L = nullptr;
		R = nullptr;
		P = nullptr;
		side = 0;
		height = 0;
		multinode = 0;

		// task-based =====
		s = 0;
		//=================
	}

	SplayTreeNode<T_KEY> *&operator[](int i) {
		switch(i % 2) {
			case 0:
				return L;
			case 1:
				return R;
			default:
				throw "[ERR]<splay>: negative index";
		}
	}

	void update() {
		int r_h = 0;
		int l_h = 0;
		T_KEY r_s = 0;
		T_KEY l_s = 0;
		if (R) {
			R->P = this;
			R->side = RIGHT;
			r_h = R->height;
			r_s = R->s;
		}
		if (L) {
			L->P = this;
			L->side = LEFT;
			l_h = L->height;
			l_s = L->s;
		}

		height = (r_h > l_h ? r_h : l_h) + 1;

  		if (P) {
  			if (P->L == this) {
  				side = LEFT;
  			} else if (P->R == this) {
  				side = RIGHT;
  			}
  		}

  		// task-based =====
		s = l_s + r_s + key;
		//=================
	}

	SplayTreeNode<T_KEY> *rotate_left() { // cares about ALL breaking connections
		SplayTreeNode<T_KEY>* node = R;
		
		R = node->L;
		node->L = this;

		node->P = P;
		if (P) {
			(*P)[side] = node;
		}
		P = node;

		this->update();
		node->update();

		if (node->P) {
			node->P->update();
			node->update();
		}

		return node;
	}

	SplayTreeNode<T_KEY> *rotate_right() { // cares about ALL breaking connections
		SplayTreeNode<T_KEY>* node = L;

		L = node->R;
		node->R = this;

		node->P = P;
		if (P) {
			(*P)[side] = node;
		}
		P = node;

		this->update();
		node->update();

		if (node->P) {
			node->P->update();
			node->update();
		}

		return node;
	}

	SplayTreeNode<T_KEY> *rotate_left_big() {
		R = R->rotate_right();
		return this->rotate_left();
	}

	SplayTreeNode<T_KEY> *rotate_right_big() {
		L = L->rotate_left();
		return this->rotate_right();
	}

	SplayTreeNode<T_KEY> *rotate(int side, int pow) {
		if (pow == SMALL) {
			if (side == LEFT) {
				return rotate_left();
			} else {
				return rotate_right();
			}
		} else if (pow == BIG) {
			if (side == LEFT) {
				return rotate_left_big();
			} else {
				return rotate_right_big();
			}
		} else {
			return this;
		}
	}

	SplayTreeNode<T_KEY> *get_parent() {
		return P;
	}

	SplayTreeNode<T_KEY> *get_grandp() {
		return P ? P->get_parent() : nullptr;
	}

	SplayTreeNode<T_KEY> *splay() {
		while(P) {
			SplayTreeNode<T_KEY> *G = get_grandp();
			if (!G) {
				P->rotate(1 ^ side, SMALL);
			} else {
				if (side == P->side) {
					G->rotate(1 ^ P->side, SMALL);
					P->rotate(1 ^ side, SMALL);
				} else {
					P->rotate(1 ^ side, SMALL);
					G->rotate(1 ^ side, SMALL);
				}
			}
		}

		update();
		return this;
	}

	SplayTreeNode<T_KEY> *insert(const T_KEY val) {
		if (key == val) {
			++cnt;
			return this;
		}

		int flag = val > key;
		(*this)[flag] = (*this)[flag] ? (*this)[flag]->insert(val) : new SplayTreeNode(val);
		
		update();
		return this;
	}

	SplayTreeNode<T_KEY> *min() {
		return L ? L->min() : this;
	}

	SplayTreeNode<T_KEY> *max() {
		return R ? R->max() : this;
	}

	SplayTreeNode<T_KEY> *find(const T_KEY val) {
		if (key == val) {
			return this;
		}

		int flag = key <= val;
		return (*this)[flag] ? (*this)[flag]->find(val) : nullptr;
	}

	SplayTreeNode<T_KEY> *next(const T_KEY val) {
		if (key == val) {
			return R ? R->min() : nullptr;
		} else if (key < val) {
			SplayTreeNode<T_KEY> *ret = R ? R->next(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			SplayTreeNode<T_KEY> *ret = L ? L->next(val) : nullptr;
			return ret ? ret : this;
		}
	}

	SplayTreeNode<T_KEY> *prev(const T_KEY val) {
		if (key == val) {
			return L ? L->max() : nullptr;
		} else if (key > val) {
			SplayTreeNode<T_KEY> *ret = L ? L->prev(val) : nullptr;
			return ret ? ret : nullptr;
		} else {
			SplayTreeNode<T_KEY> *ret = R ? R->prev(val) : nullptr;
			return ret ? ret : this;
		}
	}

	void dump(int depth = 0) {
		SplayTreeNode_dump(this, depth, depth);
	}

	void space_dump() {
		if (L) L->space_dump();
		printf("%lld ", key);
		if (R) R->space_dump();
	}

	T_KEY get_sum() {
		T_KEY s = 0;
		if (L) {
			s += L->get_sum();
		}
		if (R) {
			s += R->get_sum();
		}
		s += key;
		return s;
	}
};

template <typename T_KEY>
class SplayTree {
private:
	SplayTreeNode<T_KEY> *root;
	size_t size;

	SplayTreeNode<T_KEY> *find_node(const T_KEY &val) {
		if (!root) {
			return nullptr;
		} else {
			return root->find(val);
		}
	}

	void splay(SplayTreeNode<T_KEY> *node) {
		root = node->splay();
	}

public:
	SplayTree<T_KEY>() {
		root = nullptr;
		size = 0;
	}

	~SplayTree<T_KEY>() {
		recursive_node_delete(root);
	}

	void set_root(SplayTreeNode<T_KEY> *node) {
		root = node;
	}

	SplayTreeNode<T_KEY> *get_root() {
		return root;
	}

	void recursive_node_delete(SplayTreeNode<T_KEY> *node) {
		if (!node) {return;}
		recursive_node_delete(node->L);
		recursive_node_delete(node->R);
		delete node;
	}

	void insert(const T_KEY key) {
		if (!root) {
			root = new SplayTreeNode<T_KEY>(key);
			++size;
		} else {
			if (find(key)) {
        		++root->cnt;
        		return;
        	}

        	SplayTree<long long> *right = split(key);
        	SplayTreeNode<long long> *node = new SplayTreeNode(key);

        	node->L = get_root();
        	node->R = right->get_root();
        	node->update();
        	right->set_root(nullptr);
        	delete right;

        	root = node;
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

		if (SplayTreeNode<T_KEY> *node = root->find(val)) {
			splay(node);
			return true;
		} else {
			return false;
		}
	}

	SplayTree *split(const T_KEY val) {
		SplayTree *right = new SplayTree();
		if (find(val)) {
			if (root->R) {
				root->R->P = nullptr;
			}
			right->set_root(root->R);
			root->R = nullptr;
			if (right->get_root()) right->get_root()->update();
			if (root) 			   root->update();
			return right;
		} else {
			T_KEY *nxt = next(val - 1);
			if (!nxt) {
				return right;
			}

			find(*nxt);
			right->set_root(root);
			root = root->L;
			right->get_root()->L = nullptr;
			
			if (root) root->P    = nullptr;
			
			if (right->get_root()) right->get_root()->update();
			if (root) 			   root->update();
		}
		return right;
	}

	SplayTree *merge(SplayTree *right) {
		if (root) {
			find(max());
			root->R = right->get_root();
		} else {
			root = right->get_root();
		}

		if (root) {
			root->update();
		}

		right->set_root(nullptr);
		return this;
	}

	T_KEY min() {
		if (root) {
			return root->min()->key;
		} else {
			throw "[ERR]<splay>: empty";
		}
	}

	T_KEY max() {
		if (root) {
			return root->max()->key;
		} else {
			throw "[ERR]<splay>: empty";
		}
	}

	T_KEY *next(const T_KEY val) {
		if (!root) {
			return nullptr;
		}

		SplayTreeNode<T_KEY> *node = root->next(val);
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

		SplayTreeNode<T_KEY> *node = root->prev(val);
		if (!node) {
			return nullptr;
		} else {
			return &node->key;
		}
	}

	size_t get_size() {
		return size;
	}

	bool empty() {
		return !size;
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

int main() {
	SplayTree<long long> tree;

	return 0;
}