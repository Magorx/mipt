/*
В компьютерной сети вашей фирмы n компьютеров. В последнее время свитч, к которому они подключены, сильно барахлит, и потому не любые два компьютера могут связаться друг с другом. Кроме того, если компьютер a обменивается информацией с компьютером b, то никакие другие компьютеры не могут в это время обмениваться информацией ни с a, ни c b. Вам необходимо вычислить максимальное количество компьютеров, которые могут одновременно участвовать в процессе обмена информацией.
*/

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

using std::vector;
using std::max;
using std::min;

const int inf = 999999999;

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
    T sum = 0;
    for (int i = 0; i < row_count; ++i) {
      for (int j = 0; j < column_count; ++j) {
        sum += matrix[i][j];
      }
    }
    return sum;
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
        printf("%c ", matrix[i][j] + '0');
      }
      printf("\n");
    }
  }
};

bool isbit(int n, int x) {
	return (n >> x) & 1;
}


int main() {
	int n = 0;
	scanf("%d", &n);
	Matrix<char> matr(n, n, 0);
	matr.read();
	// matr.print();

	vector<long long> dp(1 << (n + 1), 0);
	int pmax = 1 << n;

	for (int mask = 0; mask < pmax; ++mask) {
		for (int v = 0; v < n; ++v) {
			if (isbit(mask, v)) continue;
			for (int u = 0; u < n; ++u) {
				if (isbit(mask, u)) continue;
				if (!matr[u][v]) continue;

				int new_mask = mask | (1 << u) | (1 << v);
				int new_cnt = dp[mask] + 1;
				if (new_cnt > dp[new_mask]) {
					dp[new_mask] = new_cnt;
				}
			}
		}
	}

	long long mx = 0;
	for (int i = 0; i < pmax; ++i) {
		mx = max(mx, dp[i]);
	}
	printf("%lld\n", mx * 2);

	return 0;
}

// dp[mask] - единички на местах вершин, которые мы учитываем в маске
// пересчитываем втупую, получаем О(2^n * n^2), очень интересная задача
