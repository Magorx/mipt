/*
На вступительном контесте в пилотную группу по программированию Вашему другу предложили реализовать структуру данных для хранения множеств чисел. Так как он специализируется на истории литературы, данную структуру придётся реализовать Вам.

Структура должна хранить m+1 множеств чисел от 0 до n, пронумерованных от 0 до m включительно, при этом одно число может принадлежать сразу нескольким множествам. Изначально все множества пустые.

Вы должны реализовать следующие операции на этой структуре:

ADD e s
Добавить в множество №s (0≤s≤m) число e (0≤e≤n).

DELETE e s
Удалить из множества №s (0≤s≤m) число e (0≤e≤n). Гарантируется, что до этого число e было помещено в множество

CLEAR s
Очистить множество №s (0≤s≤m).

LISTSET s
Показать содержимое множества №s (0≤s≤m) в возрастающем порядке, либо −1, если множество пусто.

LISTSETSOF e
Показать множества, в которых лежит число e (0≤e≤n), либо −1, если этого числа нет ни в одном множестве.
*/

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cctype>

// AVL_TREE ===================================================================
template <typename T_KEY>
class AVLTreeNode;

template <typename T_KEY>
long long AVLTreeNode_get_balance(const AVLTreeNode<T_KEY> *cake) {
	if (cake == nullptr) {
		return 0;
	} else {
		long long r = cake->R == nullptr ? 0 : cake->R->height;
	  	long long l = cake->L == nullptr ? 0 : cake->L->height;
	  	return r - l;
	}
}

template <typename T_KEY>
void AVLTreeNode_dump(const AVLTreeNode<T_KEY> *cake, const long long depth, const long long to_format_cnt) {
	if (!cake) {return;}

	AVLTreeNode_dump(cake->R, depth + 1, to_format_cnt + 1);

	for (long long i = 0; i < depth; ++i) {
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
	long long height;
	long long cnt;
	bool multinode;

	AVLTreeNode<T_KEY>() {
		long long cnt = 0;
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

	AVLTreeNode<T_KEY> *&operator[](long long i) {
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
		long long r = R ? R->height : 0;
  		long long l = L ? L->height : 0;
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
		long long balance = AVLTreeNode_get_balance(this);
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

		long long flag = val > key;
		(*this)[flag] = (*this)[flag] ? (*this)[flag]->insert(val) : new AVLTreeNode(val);
		
		update();
		return balance();
	}

	AVLTreeNode<T_KEY> *dislink(const T_KEY val) {
		if (val == key) {
			return R ? R : L;
		} else {
			long long flag = val > key;
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

		long long flag = key <= val;
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

	void dump(long long depth = 0) {
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

	long long find(const T_KEY &val) {
		if (!root) {
			return 0;
		}

		if (AVLTreeNode<T_KEY> *node = root->find(val)) {
			return node->cnt;
		} else {
			return 0;
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
long long AVLMapNode_get_balance(const AVLMapNode<T_KEY, T_VAL> *cake) {
	if (cake == nullptr) {
		return 0;
	} else {
		long long r = cake->R == nullptr ? 0 : cake->R->height;
	  	long long l = cake->L == nullptr ? 0 : cake->L->height;
	  	return r - l;
	}
}

template <typename T_KEY, typename T_VAL>
void AVLMapNode_dump(const AVLMapNode<T_KEY, T_VAL> *cake, const long long depth, const long long to_format_cnt) {
	if (!cake) {return;}

	AVLMapNode_dump(cake->R, depth + 1, to_format_cnt + 1);

	for (long long i = 0; i < depth; ++i) {
		printf("    ");
		if (depth - to_format_cnt- 1 <= i) {
			printf("|");
		} else {
			printf(" ");
		}
	}

	printf("%03lld>[%lld]|\n", cake->key, cake->val);
	AVLMapNode_dump(cake->L, depth + 1, to_format_cnt + 1);
}

template <typename T_KEY, typename T_VAL>
class AVLMapNode {
public:
	T_KEY key;
	T_VAL val;
	AVLMapNode<T_KEY, T_VAL> *L;
	AVLMapNode<T_KEY, T_VAL> *R;
	long long height;
	long long cnt;
	bool multinode;

	AVLMapNode<T_KEY, T_VAL>() {
		long long cnt = 0;
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

	AVLMapNode<T_KEY, T_VAL> *&operator[](long long i) {
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
		long long r = R ? R->height : 0;
  		long long l = L ? L->height : 0;
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
		long long balance = AVLMapNode_get_balance(this);
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

		long long flag = new_key > key;
		(*this)[flag] = (*this)[flag] ? (*this)[flag]->insert(new_key, new_val) : new AVLMapNode(new_key, new_val);
		
		update();
		return balance();
	}

	AVLMapNode<T_KEY, T_VAL> *dislink(const T_KEY dislink_key) {
		if (dislink_key == key) {
			return R ? R : L;
		} else {
			long long flag = dislink_key > key;
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

		long long flag = key <= val;
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

	void dump(long long depth = 0) {
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

	T_VAL &get(const T_KEY &key) {
		AVLMapNode<T_KEY, T_VAL> *node = find_node(key);
		if (node) {
			return node->val;
		} else {
			throw "AVLMap doesn't containt key";
		}
	}

	T_KEY &get_key(const T_KEY &key) {
		AVLMapNode<T_KEY, T_VAL> *node = find_node(key);
		if (node) {
			return node->key;
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

	T_KEY min() {
		if (root) {
			return root->min()->key;
		} else {
			throw "AVLMap is empty";
		}
	}

	T_KEY max() {
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

	size_t get_size() {
		return size;
	}

	void dump() {
		if (root) {
			root->dump();
		}
	}
};

//=============================================================================
// Будем хранить массив сетов для быстрого доступа к ним
// И мапу из числа в сет указателей на сеты, в которых это число лежит

template <typename T>
const T &min(const T &first, const T &second) {
	return first < second ? first : second;
}

void putin_local(AVLMap<long long, long long> &loc, long long dist, long long x) {
	if (loc.find(dist)) {
		dist = loc.max() + 1;
	}
	loc.insert(dist, x);
}

void put_car_on_pol(AVLMap<long long, long long*> &cur, AVLMap<long long, long long> &loc, AVLTree<long long> *glob, long long x) {
	long long next_pos = glob[x].get_size() ? glob[x].min() : 999999999;
	
	putin_local(loc, next_pos, x);
	cur.insert(x, &(loc.get_key(next_pos)));
}

void get_car_from_pol(AVLMap<long long, long long*> &cur, AVLMap<long long, long long> &loc) {
	long long max_dist = loc.max();
	long long max_car = loc.get(max_dist);
	long long x = max_car;
	
	loc.erase(max_dist);
	cur.erase(x);
}

// ============================= FAST INPUT DLYA NISHIH =================== 
// Когда в следующий раз потребуется вводить через буфер, напишу библиотеку для нормального вида
// А сейчас да, выглядит ужасно, ещё и неправда
long long read_next_long_long(char **buffer) {
	char *c = *buffer;
	while (c && isspace(*c)) ++c;

	long long l = 0;
	while (isdigit(*c)) {
		l = l * 10 + *c - '0';
		++c;
	}

	*buffer = c;
	return l;
}
// ========================================================================

// Идея: будем хранить мап 'локальных' машинок - которые уже на полу, с указанием, сколько расстояния до
// её след. вхождения + мап для локальных в обратную сторону: из расстояния в машинку на полу. беря в нём
// максимум будет понимать, какую машинку убрать.
// Ну и да, в основе жадник: будем с пола убирать ту машинку, с которой не будем играть дольше всего... логично же

int main() {
// ============================= FAST INPUT DLYA NISHIH =================== 
	FILE *fin = stdin;

    fseek(fin, 0, SEEK_END);
    long input_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    char *buffer = (char*) calloc(input_size + 1, sizeof(char));
    char *input_init_pointer_for_free = buffer;
    fread(buffer, sizeof(char), input_size, fin);
    fclose(fin);
// ========================================================================

	long long n, k, p;
	n = read_next_long_long(&buffer);
	k = read_next_long_long(&buffer);
	p = read_next_long_long(&buffer);

	AVLMap<long long, long long> local_dist; // next pos in global -> car
	AVLTree<long long> *global_dist = (AVLTree<long long>*) calloc(n + 1, sizeof(AVLTree<long long>)); // car -> all poses

	AVLMap<long long, long long*> cur_cars; // current cars on pol

	long long *arr = (long long*) calloc(p, sizeof(long long));
	
	for (long long i = 0; i < p; ++i) {
		long long &x = arr[i];
		x = read_next_long_long(&buffer);
		global_dist[x].insert(i);
	}

	long long ret = 0;
	for (long long i = 0; i < p; ++i) {
		long long x = arr[i];
		global_dist[x].erase(global_dist[x].min());

		if (cur_cars.find(x)) { // если такая машинка есть, обновим расстояние до след. её вхождения
			long long car_dist = *cur_cars.get(x);
			local_dist.erase(car_dist);
			car_dist = global_dist[x].get_size() ? global_dist[x].min() : 999999999;
			putin_local(local_dist, car_dist, x);
			continue;
		} else {
			++ret;
		}

		if (cur_cars.get_size() == k) {
			get_car_from_pol(cur_cars, local_dist);
		}

		put_car_on_pol(cur_cars, local_dist, global_dist, x);

	}

	printf("%lld\n", ret);

	free(arr);
	free(global_dist);
	free(input_init_pointer_for_free);
	return 0;
}

// AVL-дерево и мап даровали нам свои логарифм для O(plogn) или чего-то подобного, логарифм там точно есть
// ввод через буфер был дарован нам своими структурами или я не понял чем, но без него был тл
