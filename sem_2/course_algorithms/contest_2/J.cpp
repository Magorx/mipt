/*
International Biology Manufacturer (IBM) обнаружили, что органический материал на Марсе имеет ДНК, состоящий из 5 символов(a,b,c,d,e), вместо четырех компонентов ДНК на Земле. Однако в строке не может встречаться ни одна из следующих пар в качестве подстроки: cd, ce, ed и ee.

IBM заинтересовались сколько правильных Марсианских строк ДНК длины n возможно составить?
*/

#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

template<typename T>
using Row = vector<T>;

template<typename T>
using MatrixMap = vector<Row<T>>;

const unsigned long long M = 999999937;

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
        sum = (sum + matrix[i][j]) % M;
      }
    }
    return sum;
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
        result.matrix[i][j] = (result.matrix[i][j] + first.matrix[i][k] * second.matrix[k][j]) % M;
      }
    }
  }
  return result;
}

int main() {
  const Matrix<unsigned long long> matr {
    5, 5,
    {
    // a  b  c  d  e
      {1, 1, 1, 1, 1}, // a
      {1, 1, 1, 1, 1}, // b
      {1, 1, 1, 0, 0}, // c
      {1, 1, 1, 1, 1}, // d
      {1, 1, 1, 0, 0}  // e
    }
  };


  unsigned long long n = 1;
  while (true) {
    scanf("%llu", &n);
    if (!n) break;

    if (n == 1) {
      printf("%llu\n", 5llu);
    } else {
      printf("%llu\n", binpow(matr, n - 1).sum());
    }
  }

  return 0;
}

// Возведем матрицу перехода между длиной n и n + 1 в степень n - 1 и получим что надо за О(logn * n^3)