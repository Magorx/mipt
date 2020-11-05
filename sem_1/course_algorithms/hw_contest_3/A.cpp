#include <cstdlib>
#include <cstdio>
#include <cassert>

template <typename T>
class Node;

template <typename T>
int get_balance(const Node<T> *cake) {
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

	T key;
	Node<T> *L;
	Node<T> *R;
	int height;

	Node<T>() {
		key = 0;
		L = nullptr;
		R = nullptr;
		height = 1;
	}

	Node<T>(const T &new_key) {
		key = new_key;
		L = nullptr;
		R = nullptr;
		height = 1;
	}

	void update() {
		int r = R == nullptr ? 0 : R->height;
  		int l = L == nullptr ? 0 : L->height;
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
		int balance = get_balance(this);
		printf("key %d -> balance %d\n", key, balance);
		assert(abs(balance) <= 2);
		if (balance == 2) {
			balance = get_balance(R);
			printf("key %d -> second balance %d\n", R->key, balance);
			if (balance == 1 || balance == 0) {
				printf("rot left\n");
				return rotate_left();
			} else {
				printf("rot left BIG\n");
				return rotate_left_big();
			}
		} else if (balance == -2) {
			balance = get_balance(L);
			if (balance == -1 || balance == 0) {
				printf("rot right\n");
				return rotate_right();
			} else {
				printf("rot right BIG\n");
				return 	rotate_right_big();
			}
		} else {
			return this;
		}
	}

	Node<T> *insert(T new_key) {
		int flag = new_key <= key;
		//Node<T> *a = (*this)[0];
		if (flag) {
			L = L == nullptr ? new Node(new_key) : L->insert(new_key);
		} else {
			R = R == nullptr ? new Node(new_key) : R->insert(new_key);
		}
		update();
		printf("BEFORE ====\n");
		dump();
		Node<T> *tmp = balance();
		printf("AFTER =====\n");
		tmp->dump();
		return tmp;
	}

	void dump(int depth = 0) {
		Node_dump(this, depth, depth);
	}

};

int main() {
	Node<int> *a = new Node<int>(6);
	//(*a)[0] = a;
	//(*a)[1] = a;

	a = a->insert(5);
	a = a->insert(7);
	a = a->insert(4);
	a = a->insert(10);
	a = a->insert(9);
	a = a->insert(2);
	a = a->insert(1);
	a = a->insert(8);
	a = a->insert(3);
	printf("======================\n");
	a->dump();
	printf("\n");

	//printf("%lld %lld\n", (*a)[0], (*a)[1]);

	return 0;
}