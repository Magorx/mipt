/*
Конь в шахматах
О(1)
*/

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>
using std::vector;
using std::queue;

const int DESK_SIZE = 8;

struct Coord {
	int x;
	int y;

	Coord(): x(-1), y(-1) {}
	Coord(int a, int b): x(a), y(b) {}

	void dump() {
		printf("%c%d\n", x + 'a', y + 1);
	}
};

bool operator==(const Coord &a, const Coord &b) {
	return a.x == b.x && a.y == b.y;
}

Coord operator+(const Coord &a, const Coord &b) {
	return {a.x + b.x, a.y + b.y};
}

const Coord SHIFTS[] = {{1, 2}, {2, 1}, {-1, 2}, {2, -1}, {1, -2}, {-2, 1}, {-1, -2}, {-2, -1}};

inline bool check_coord(const Coord &a) {
	return a.x >= 0 && a.y >= 0 && a.x < DESK_SIZE && a.y < DESK_SIZE;
}

vector<Coord> get_path(vector<vector<Coord>> p, Coord t) {
	vector<Coord> ret;
	while (!(p[t.x][t.y] == t)) {
		ret.push_back(t);
		t = p[t.x][t.y];
	}
	return ret;
}

vector<vector<Coord>> nabeg_mongol(Coord s, Coord t) {
	vector<vector<Coord>> p(DESK_SIZE, vector<Coord>(DESK_SIZE));
	queue<Coord> q;
	q.push(s);
	p[s.x][s.y] = {s.x, s.y};

	while (true) {
		Coord cur = q.front();
		q.pop();
		if (cur == t) {
			break;
		}

		for (int i = 0; i < 8; ++i) {
			Coord now = cur + SHIFTS[i];

			if (check_coord(now) && p[now.x][now.y] == Coord(-1, -1)) {
				p[now.x][now.y] = cur;
				q.push(now);
			}
		}
	}

	return p;
}

int main() {
    char c;
    int x;

    scanf("%c%d\n", &c, &x);
    Coord s = {c - 'a', x - 1};

    scanf("%c%d", &c, &x);
    Coord f = {c - 'a', x - 1};

    auto ans = get_path(nabeg_mongol(s, f), f);
    s.dump();
    for (int i = ans.size() - 1; i >= 0; --i) {
    	ans[i].dump();
    }

    return 0;
}
