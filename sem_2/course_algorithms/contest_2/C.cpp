/*
Крис Юрьевич Каменев на вокзале решил сыграть в "Трехкарточный Монти", хотя его мать запретила ему это делать. В этой игре участвуют три карты, выставленные в ряд слева направо. Изначально карта с валетом, которую нужно угадать, находится под стоящей посередине чашкой.

Затем, за один ход карточный мошенник меняет местами карту посередине с одной из двух других карт случайным образом (он выберет каждую с одинаковой вероятностью), так что выбранная карта становится посередине. Игра длится n ходов, и в каждом из них мошенник независимо выбирает карту для обмена со средней на каждом ходу.

После n-го хода мошенник просит Криса указать на карту с валетом, и он указывает на стоящую посередине карту, но Крис отвлекся во время игры и не уверен, находится ли там карта или нет. Поэтому он просит вас сообщить ему вероятность того, что карта с валетом действительно посередине.

Число ходов n может быть очень большим, поэтому Крис не сообщил вам его. Вместо этого он дал вам массив a1,a2,...,ak, такой, что
*/

#include <stdlib.h>
#include <stdio.h>
#include <vector>
 
using std::vector;
 
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
  T matrix[2][2];
 
  Matrix() {
    row_count = 0;
    column_count = 0;
    matrix[0][0] = 0;
    matrix[0][1] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = 0;
  }
 
  Matrix(int _row_count, int _column_count) {
    row_count = _row_count;
    column_count = _column_count;
    matrix[0][0] = 0;
    matrix[0][1] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = 0;
  }
 
  Matrix(int _row_count, int _column_count, const long long matr[2][2]) {
    row_count = _row_count;
    column_count = _column_count;
    matrix[0][0] = matr[0][0];
    matrix[0][1] = matr[0][1];
    matrix[1][0] = matr[1][0];
    matrix[1][1] = matr[1][1];
  }

  Matrix(const Matrix &other) {
    row_count = other.row_count;
    column_count = other.column_count;
    matrix[0][0] = other.matrix[0][0];
    matrix[0][1] = other.matrix[0][1];
    matrix[1][0] = other.matrix[1][0];
    matrix[1][1] = other.matrix[1][1];
  }

  Matrix& operator=(const Matrix &other) {
    row_count = other.row_count;
    column_count = other.column_count;
    matrix[0][0] = other.matrix[0][0];
    matrix[0][1] = other.matrix[0][1];
    matrix[1][0] = other.matrix[1][0];
    matrix[1][1] = other.matrix[1][1];
    return *this;
  }
 
  T *operator[](const int i) {
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
Matrix<T> operator*(Matrix<T> &first, Matrix<T> &second) {
  if (first.column_count != second.row_count) {
    return {0, 0, {}};
  }
  Matrix<T> result(first.row_count, second.column_count);
 
  result[0][0] = (first[0][0] * second[0][0] + first[0][1] * second[1][0]) % MOD;
  result[1][0] = (first[1][0] * second[0][0] + first[1][1] * second[1][0]) % MOD;
  result[0][1] = (first[0][0] * second[0][1] + first[0][1] * second[1][1]) % MOD;
  result[1][1] = (first[1][0] * second[0][1] + first[1][1] * second[1][1]) % MOD;
  return result;
}
 
template <typename T>
T big_binpow(const T &obj, long long pow) {
  T ans = T::E(obj.row_count);
  T powed = obj;
  while (pow) {
    if (pow & 1) {
      ans = ans * powed;
    }
    powed = powed * powed;
    pow >>= 1;
  }
  return ans;
}
 
long long big_bin_pow(long long x, long long pow) {
    long long answer = 1;
    long long powed = x;
    while(pow) {
      if(pow & 1) {
        answer = (answer * powed) % MOD;
      }

      powed = (powed * powed) % MOD;
      pow >>= 1;
    }
 
    return answer;
}
 
int main() {
  long long k = 0;
  scanf("%lld", &k);

  vector<long long> arr(k, 0);
  for(long long i = 0; i < k; ++i) {
    scanf("%lld", &arr[i]);
  }

  Matrix<long long> matr(2, 2);
  matr[0][0] = 1;
  matr[0][1] = 2;
  matr[1][0] = 1;
  matr[1][1] = 0;

  long long divider = 2;
  for (int i = 0; i < k; ++i) {
    auto tmp = big_binpow(matr, arr[i]);
    // matr.destr();
    matr = tmp;
    divider = big_bin_pow(divider, arr[i]);
  }

  auto res =  matr[1][1];
  // res.print();
  printf("%lld/%lld\n", (res * 500000004) % MOD, (divider * 500000004) % MOD);

  return 0;
}

// Если выписать ответы для малых n, то становится ясна закономерность, которую можно выразить в матричках
// Единственная проблема - нужно вовзводить не в n степень, а в n - 1, решается, по словам умных математиков,
// умножением на 500000004. Потом убираем все каллоки, и прога заходит за O(n * log(a))
