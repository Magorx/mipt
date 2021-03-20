/*
Филипп любит гулять по своему городу, но знает, что шпионы из ЛесТеха могут быть где угодно, поэтому он хочет узнать, по скольким различным путям он может пройти по городу Долгопрудный. Город Филиппа состоит из всех точек (x,y) на плоскости таких, что x и y неотрицательны. Филипп должен начать прогулку в начале координат (точке (0,0)) и должен закончить путь в точке (k,0). Если Филипп сейчас находится в точке (x,y), то за один шаг он может перейти в точку (x+1,y+1),(x+1,y) или (x+1,y−1).

Кроме того, существуют n горизонтальных отрезков, i-й из которых идет от точки x=ai до x=bi, включительно и располагается в y=ci. Гарантируется, что a1=0, an≤k≤bn, и ai=bi−1 для всех 2≤i≤n. i-й из этих отрезков заставляет Филиппа находиться только в точках с y координатой в отрезке 0≤y≤ci, когда его x координата находится в отрезке ai≤x≤bi. Заметьте, что когда один отрезок кончается, а другой начинается, то он должен находиться под обоими отрезками одновременно.

Филипп хочет узнать, сколько существует различных путей (последовательностей шагов) из начала координат в точку (k,0), удовлетворяющих этим ограничениям, по модулю 109+7.
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

const int BIBASE = 10;
class BigInt {
public:
	vector<int> digits;

	BigInt():
	digits()
	{}

	BigInt(int x) {
		digits.resize(0);
		while (x) {
			digits.push_back(x % BIBASE);
			x /= BIBASE;
		}
		reverse(digits.begin(), digits.end());
	}

	void read_by_scanf() {
		digits.resize(0);
		char d = ' ';
		while (isspace(d)) {
			scanf("%c", &d);
		}
		while (isdigit(d)) {
			digits.push_back(d - '0');
			scanf("%c", &d);
		}
	}

	void print(char end_symb = ' ') const {
		int l = digits.size();
		for (int i = 0; i < l; ++i) {
			printf("%d", digits[i]);
		}
		printf("%c", end_symb);
	}

  void fix() {
    if (digits[0]) {
      return;
    }

    reverse(digits.begin(), digits.end());
    while (digits.size() > 1 && !digits[digits.size() - 1]) {
      digits.pop_back();
    }
    reverse(digits.begin(), digits.end());
  }

	BigInt &operator=(const BigInt &other) {
		digits = other.digits;
		return *this;
	}

  void dec_1() {
    int i = digits.size() - 1;
    for (; i >= 0; --i) {
      if (digits[i]) {
        break;
      }
    }

    digits[i] -= 1;
    ++i;
    for (; i < digits.size(); ++i) {
      digits[i] = BIBASE - 1;
    }

    fix();
  }

  void div_2() {
    int rem = 0;
    for (int i = 0; i < digits.size(); ++i) {
      int cur = digits[i] + rem * BIBASE;
      digits[i] = cur / 2;
      rem = cur % 2;
    }
    fix();
  }

  bool is_1() {
  	return digits.size() == 1 && digits[0] == 1;
  }

  bool is_0() {
  	return digits.size() == 1 && digits[0] == 0;
  }

  bool is_even() {
  	if (digits.size() == 0) {
  		printf("EROR\n");
  	}
  	return digits[digits.size() - 1] % 2 == 0;
  }
};

bool isbit(int n, int x) {
  return (n >> x) & 1;
}

bool ok_seq(const int m1, const int m2, const int len=32) {
  for (int x = 0; x < len - 1; ++x) {
    char cur_bit = isbit(m1, x);
    char cnt = 1 + (cur_bit == isbit(m1, x + 1)) + (cur_bit == isbit(m2, x)) + (cur_bit == isbit(m2, x + 1));
    if (cnt == 4) {
      return false;
    }
  }
  return true;
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

const int MAX_C = 17;
int main() {
  vector<Matrix<long long>> matrs;
  for (int i = 0; i <= MAX_C; ++i) {
    Matrix<long long> matr(MAX_C, MAX_C, 0);
    for (int j = MAX_C - 1; j > MAX_C - i - 1; --j) {
      for (int a = 0; a < 2; ++a) {
        for (int b = 0; b < 2; ++b) {
          int x = j + a;
          int y = j + b;
          if (x >= 0 && x < MAX_C && y >= 0 && y < MAX_C) {
            matr[x][y] = 1;
          }
        }
      }
    }
    // printf("%d)\n", i);
    // matr.print();
    // printf("===\n");
    matrs.push_back(matr);
  }

  Matrix<long long> cur(MAX_C, 1, 0);
  Matrix<long long> last_matr(MAX_C, 1, 0);
  cur[MAX_C - 1][0] = 1;
  last_matr[MAX_C - 1][0] = 1;

  long long n, k;
  scanf("%lld %lld", &n, &k);

  for (int i = 0; i < n; ++i) {
    long long a, b, c;
    scanf("%lld %lld %lld", &a, &b, &c);

    if (b >= k) {
      b = k;
    }

    long long l = b - a;
    ++c;
    Matrix<long long> muled = big_binpow(matrs[c], l);
    cur = muled * cur;
  }

  // cur = matrs[1] * cur;
  printf("%lld\n", cur[MAX_C - 1][0]);

	return 0;
}

// cur - столбец высоты 17, в котором i-я снизу клеточка хранит кол-во способов добраться до нее из (0, 0)
// легко заметить формулу пересчета таких столбцов из предыдущих и заметить умножение матриц, осталось
// предподсчитать их на всякий случай и получить что-то вроде O(n * 17^2 * logk)