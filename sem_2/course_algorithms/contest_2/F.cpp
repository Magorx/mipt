/*
Найти мин гамиьтонов путь
Продавец аквариумов для кошек хочет объехать n городов, посетив каждый из них ровно один раз. Помогите ему найти кратчайший путь.
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
    for (int i = 0; i < row_count; ++i) {
      for (int j = 0; j < column_count; ++j) {
        scanf("%lld", &matrix[i][j]);
      }
    }
  }

  void print() {
    for (int i = 0; i < row_count; ++i) {
      for (int j = 0; j < column_count; ++j) {
        printf("%lld ", matrix[i][j] == +inf ? -1 : matrix[i][j]);
      }
      printf("\n");
    }
  }
};

bool isbit(int n, int x) {
  return (n >> x) & 1;
}

struct Pair {
  int x;
  int y;
};

int main() {
  int n = 0;
  scanf("%d", &n);
  Matrix<long long> matr(n, n, 0);
  matr.read();

  Matrix<long long> dp(n, 1 << (n + 1), +inf);
  for (int i = 0; i < n; ++i) {
    dp[i][1 << i] = 0;
  }
  Matrix<Pair> pp(n, 1 << (n + 1), {-1, -1});
  int pmax = 1 << n;

  for (int mask = 1; mask < pmax; ++mask) {
    for (int v = 0; v < n; ++v) {
      for (int u = 0; u < n; ++u) {
        if (isbit(mask, u)) continue;

        int new_mask = mask | (1 << u);
        int new_cost = dp[v][mask] + matr[v][u];
        if (new_cost < dp[u][new_mask]) {
          dp[u][new_mask] = new_cost;
          pp[u][new_mask] = {v, mask};
        }
      }
    }
  }

  long long mn = 999999999;
  int mi = 0;
  for (int i = 0; i < n; ++i) {
    if (mn > dp[i][(1 << n) - 1]) {
      mn = dp[i][(1 << n) - 1];
      mi = i;
    }
  }
  printf("%lld\n", mn);

  int v = mi;
  int msk = (1 << n) - 1;
  while (v >= 0) {
    printf("%d\n", v + 1);
    int nmsk = pp[v][msk].y;
    int nv = pp[v][msk].x;
    msk = nmsk;
    v = nv;
  }
  printf("\n");

  return 0;
}

// dp[i][mask] - мин длина пути из вершины v по вершинам, включенным в маске
// пересчитываем и побеждаем за О(2^n * n^2)