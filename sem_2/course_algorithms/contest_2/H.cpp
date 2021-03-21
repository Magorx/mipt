/*
До конца света осталось не так много дней, но Третий всадник Апокалипсиса Голод так и не смог понять, какие города стоит наказывать за прегрешения, а какие нет. Тогда он решил потренироваться на одной небольшой стране.

Эта страна представляет собой клетчатый прямоугольник размера n×m, в котором каждая клетка — отдельный город. Голод может либо наслать голод на город, либо пощадить его. При этом есть города, в которых еды хватит и до следующего конца света, и Голод не в силах заставить их голодать, а есть те, в которых еды никогда и не было, и им в любом случае придется голодать.

Страшный Суд Голода должен быть ещё и справедлив, а это значит, что в любом квадрате размера 2×2 должно быть поровну голодающих и сытых городов. Теперь Голод хочет узнать количество различных вариантов распределения городов этой страны на голодающие и сытые.
*/

#include <stdlib.h>
#include <stdio.h>
#include <vector>

using std::vector;
using std::swap;

const long long MOD = 1000000007;

template<typename T>
using Row = vector<T>;

template<typename T>
using MatrixMap = vector<Row<T>>;

template <typename T>
T binpow(const T &obj, long long pow) {
  if (pow == 1) {
    return obj;
  } else {
    if (pow % 2) {
      return binpow(obj, pow - 1) * obj;
    } else {
      T tmp = binpow(obj, pow / 2);
      return tmp * tmp;
    }
  }
}

template <typename T>
struct Matrix {
  long long row_count;
  long long column_count;
  MatrixMap<T> matrix;

  Matrix() {
    row_count = 0;
    column_count = 0;
    matrix = {};
  }

  Matrix(int _row_count, int _column_count) {
    row_count = _row_count;
    column_count = _column_count;
    matrix = Row<Row<T>> (_row_count, Row<T>(_column_count, 0));
  }

  Matrix(int _row_count, int _column_count, const T neutral) {
    row_count = _row_count;
    column_count = _column_count;
    matrix = Row<Row<T>> (_row_count, Row<T>(_column_count, neutral));
  }

  Matrix(int _row_count, int _column_count, const MatrixMap<T>& _matrix) {
    row_count = _row_count;
    column_count = _column_count;
    matrix = _matrix;
  }

  Row<T> &operator[](const int i) {
    return matrix[i];
  }

  static Matrix<T> E(const int n) {
    Matrix<T> res(n, n);
    for (int i = 0; i < n; ++i) {
      res[i][i] = 1;
    }
    return res;
  }

  T sum() {
    T ret = 0;
    for (int i = 0; i < row_count; ++i) {
      for (int j = 0; j < column_count; ++j) {
        ret = (ret + matrix[i][j]) % MOD;
      }
    }
    return ret;
  }

  void read() {
  	char c = 0;
  	scanf("%c", &c);
  	for (int i = 0; i < row_count; ++i) {
      for (int j = 0; j < column_count; ++j) {
        scanf("%c", &c);
        matrix[i][j] = c == 'Y' ? 1 : 0;
      }
      scanf("%c", &c);
    }
  }

  void print() {
  	for (int i = 0; i < row_count; ++i) {
      for (int j = 0; j < column_count; ++j) {
        printf("%lld ", matrix[i][j]);
      }
      printf("\n");
    }
  }
};

struct Pair {
	int to_and;
	int to_or;
};

int set_bit(int x, int n, int bit) {
	int a = 1 << n;
	return bit ? (x | a) : (x & (~a));
}

bool has_two_bits(int mask, int len = 0) {
	for (int i = 0; i < len - 1; ++i) {
		if ((mask & 1) == (((mask >> 1) & 1))) {
			return true;
		}
		mask >>= 1;
	}
	return false;
}

int main() {
	int n = 0;
	int m = 0;
	scanf("%d %d\n", &n, &m);
	Matrix<char> map(n, m);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			scanf("%c", &map[i][j]);
		}
		char c;
		scanf("%c", &c);
	}

	vector<Pair> helper(m);
	for (int i = 0; i < m; ++i) {
		int to_and = ~0;
		int to_or  =  0;
		for (int j = 0; j < n; ++j) {
			if (map[j][i] == '+') {
				to_or = set_bit(to_or, j, 1);
			} else if (map[j][i] == '-') {
				to_and = set_bit(to_and, j, 0);
			}
		}
		helper[i] = {to_and, to_or};
	}

	long long maxp = 1 << n;
	long long *prev = (long long*) calloc(maxp + 1, sizeof(long long));
	long long *dp   = (long long*) calloc(maxp + 1, sizeof(long long));

	for (int i = 0; i < maxp; ++i) {
		int mask = (i | helper[0].to_or) & helper[0].to_and;
		prev[mask] = 1;
	}

	for (int i = 1; i < m; ++i) {
		for (int msk = 0; msk < maxp; ++msk) {
			int mask = (msk | helper[i].to_or) & helper[i].to_and;
			if (has_two_bits(mask, n)) {
				dp[mask] = prev[mask ^ (maxp - 1)];
			} else {
				dp[mask] = (prev[mask] + prev[mask ^ (maxp - 1)]) % MOD;
			}
		}
		for (int msk = 0; msk < maxp; ++msk) prev[msk] = 0;
		swap(prev, dp);
	}

	long long sum = 0;
	std::vector<bool> used(maxp, 0);
	for (int i = 0; i < maxp; ++i) {
		int mask = (i | helper[m - 1].to_or) & helper[m - 1].to_and;
		if (used[mask]) {
			continue;
		} else {
			used[mask] = 1;
		}
		sum = (sum + prev[mask]) % MOD;
	}

	printf("%lld\n", sum);

	return 0;
}

// Заметим, что к каждой маске с прошлого слоя можно прийти либо из такой же, либо из инверснутой
// При этом если есть два подряд идущих бита одного цвета - то только из инверса
// Ускоряем ограничение плюсами и минусами, превращая их в битовые операции
// И в итоге залетает за O(m * 2^n) или 45мс
