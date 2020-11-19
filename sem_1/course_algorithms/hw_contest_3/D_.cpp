struct Node;
ll node_size(Node* n);
ll node_max(Node* n);
void print_tree(Node* n);
const ll inf = 1e10;

ll randint() {
  return (rand() << 16) ^ rand();
}


struct Node {
    ll x;
    ll y;
    Node *L;
    Node *R;

    ll cnt;
    ll max_x;
    ll d;

    Node(ll _x) {
        x = _x;
        max_x = x;
        d = 0;
        y = randint();
        L = nullptr;
        R = nullptr;
        cnt = 1;
    }

    void update() {
        cnt = node_size(L) + node_size(R) + 1;
        max_x = max(max(node_max(L), node_max(R)), x);
    }

    void push() {
        x += d;
        if (R) {
            R->d += d;
        }
        if (L) {
            L->d += d;
        }
        d = 0;
    }
};

ll node_size(Node* n) {
    if (!n) {
        return 0;
    } else {
        return n->cnt;
    }
}

ll node_max(Node* n) {
    if (!n) {
        return -inf;
    } else {
        return n->max_x + n->d;
    }
}

pair<Node*, Node*> split(Node *n, ll k) {
    if (!n) {
        return {nullptr, nullptr};
    }

    ll left_size = node_size(n->L);
    if (left_size < k) {
        n->push();
        pair<Node*, Node*> ret = split(n->R, k - left_size - 1);
        n->R = ret.first;
        n->update();
        return {n, ret.second};
    } else {
        n->push();
        pair<Node*, Node*> ret = split(n->L, k);
        n->L = ret.second;
        n->update();
        return {ret.first, n};
    }
}

Node* merge(Node *L, Node *R) {
    if (!L) {
        return R;
    } else if (!R) {
        return L;
    }

    if (L->y > R->y) {
        L->push();
        L->R = merge(L->R, R);
        L->update();
        return L;
    } else {
        R->push();
        R->L = merge(L, R->L);
        R->update();
        return R;
    }
}

void print_tree(Node* n) {
  if (!n) {
    return;
  }

  print_tree(n->L);
  cout << n->x << '(' << n->d << ')' << ' ';
  print_tree(n->R);
}

