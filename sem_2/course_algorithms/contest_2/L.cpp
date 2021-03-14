/*
Симпатичные узоры c n = 10^100
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
int MOD = 0;

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
T big_binpow(const T &obj, BigInt &pow) {
	T ans = T::E(obj.row_count);
	T powed = obj;
	while (!pow.is_0()) {
		if (!pow.is_even()) {
			ans = ans * powed;
		}
		powed = powed * powed;
		pow.div_2();
	}
	return ans;
}

int main() {
  BigInt n;
  n.read_by_scanf();
  n.dec_1();
  int m, mod;
  scanf("%d %d", &m, &mod);
  MOD = mod;

  if (n.is_0()) {
  	printf("%d\n", (1 << m) % mod);
  	return 0;
  }

  int mmask = (1 << m);

  Matrix<long long> dp(m + 1, mmask + 1, 0);
  for (int i = 0; i < mmask; ++i) {
    dp[1][i] = 1;
  }

  Matrix<long long> correct(mmask + 1, mmask + 1, 0);
  for (int i = 0; i < mmask; ++i) {
    for (int j = 0; j < mmask; ++j) {
      correct[i][j] = ok_seq(i, j, m);
    }
  }

  Matrix<long long> powed = big_binpow(correct, n);

  long long ans = powed.sum() % mod;

  printf("%lld\n", ans);

	return 0;
}

// матрички туда-сюда перемножили и O(8^m*m)