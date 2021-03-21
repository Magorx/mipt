/*
Гномы прорыли очень удобную для них систему тоннелей в пещерах. У неё есть только один вход и один выход. Равноудаленные от входа по количеству пройденных тоннелей пещеры на языке гномов называется обскварк.

В системе тоннелей есть L обскварков и N пещер на каждом из них. Рассмотрим два соседних обскварка L1 и L2. Чтобы можно было быстро передвигаться в тоннелях, каждая пещера из уровня L1 соединена с каждой пещерой из уровня L2. Для перехода из i-й перещы первого обскварка в j-ю пещеру второго (i,j∈{1, 2, …, N}) необходимо cij миллилитров масла в фонаре. Для каждой пары соседних обскварков это количество одинаковое. Также количество масла на переход в каждую пещеру L2 тоже одинаковое для всех пещер из L1, то есть cij одинаково для i∈{1, 2, …, N} и фиксированного j.

У Гимли есть бутылки для масла емкостью M миллилитров. На свой путь по таким пещерам он хочет взять с собой несколько полных бутылочек с маслом. Гимли просит вас помочь ему и найти количество путей между точками входа и выхода таких, что суммарное количество масла будет делиться на заданное число M.
*/

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <vector>
#include <algorithm>

using std::vector;
using std::max;
using std::min;
using std::reverse;
using std::swap;

const int inf = 999999999;
int MOD = 1000000007;

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

template <typename T>
Matrix<T> operator*(const Matrix<T> &first, const Matrix<T> &second) {
  if (first.column_count != second.row_count) {
    return {0, 0, {}};
  }
  Matrix<T> result(first.row_count, second.column_count);

  for (int i = 0; i < first.row_count; ++i) {
    for (int j = 0; j < second.column_count; ++j) {
      for (int k = 0; k < first.column_count; ++k) {
        result.matrix[i][j] = (result.matrix[i][j] + first.matrix[i][k] * second.matrix[k][j]) % MOD;
      }
    }
  }
  return result;
}

template <typename T>
T big_binpow(const T &obj, long long pow) {
  T ans = T::E(obj.row_count);
  T powed = obj;
  while (pow) {
    if (pow % 2) {
      ans = ans * powed;
    }
    powed = powed * powed;
    pow /= 2;
  }
  return ans;
}

template <typename T>
Row<T> shifted_down(const Row<T> row) {
  int l = row.size();
  Row<T> ret(l);
  for (int i = 0; i < l; ++i) {
    ret[i] = row[(i - 1 + l) % l];
  }
  return ret;
}

int main() {
  long long n, l, m;
  scanf("%lld %lld %lld", &n, &l, &m);

  Matrix<long long> matr(m ,m, 0);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < m; ++j) {
      matr[i][j] = (i + j) % m;
    }
  }

  Matrix<long long> vhod(n, 1, 0);
  for (int i = 0; i < n; ++i) {
    scanf("%lld", &vhod[i][0]);
  }

  Matrix<long long> obsc(n, 1, 0);
  for (int i = 0; i < n; ++i) {
    scanf("%lld", &obsc[i][0]);
  }

  Matrix<long long> vihod(n, 1, 0);
  for (int i = 0; i < n; ++i) {
    scanf("%lld", &vihod[i][0]);
  }

  Matrix<long long> inp(m, 1, 0);
  for (int i = 0; i < n; ++i) {
    inp[vhod[i][0] % m][0] += 1;
  }

  Matrix<long long> dp(m, m, 0);
  for (int j = 0; j < n; ++j) {
    dp[0][((m - obsc[j][0]) % m + m) % m] += 1;
  }

  for (int i = 1; i < m; ++i) {
    dp[i] = shifted_down(dp[i - 1]);
  }
  dp.print();

  dp = big_binpow(dp, l - 2);
  auto cur = dp * inp;

  long long ans = 0;
  for (int i = 0; i < n; ++i) {
    ans = (ans + cur[((m - obsc[i][0] - vihod[i][0]) % m + m) % m ][0]) % MOD;
  }

  printf("%lld\n", ans);

  return 0;
}

// Заметим, что переходы между ОБСКВАРКАМИ очень одинаковые внутри пещер, а связи между ними - полные
// Напрашивается умножение матриц с рассматриванием предпоследнего столбца отдельно
// Итого: O(m^2*log(l))
