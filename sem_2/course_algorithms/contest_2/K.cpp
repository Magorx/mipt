/*
Симпатичные узоры
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

int main() {
  int n = 0;
  int m = 0;
  scanf("%d %d", &n, &m);
  if (m < n) {
    swap(n, m);
  }

  int mmask = (1 << n);

  Matrix<long long> dp(m + 1, mmask + 1, 0);
  for (int i = 0; i < mmask; ++i) {
    dp[1][i] = 1;
  }

  Matrix<char> correct(mmask + 1, mmask + 1, 0);
  for (int i = 0; i < mmask; ++i) {
    for (int j = 0; j < mmask; ++j) {
      correct[i][j] = ok_seq(i, j, n);
    }
  }

  for (int j = 1; j < m; ++j) {
    for (int mask = 0; mask < mmask; ++mask) {
      for (int next_mask = 0; next_mask < mmask; ++next_mask) {
        if (correct[mask][next_mask]) {
          dp[j + 1][next_mask] += dp[j][mask];
        }
      }
    }
  }

  int ans = 0;
  for (int i = 0; i < mmask; ++i) {
    ans += dp[m][i];
  }

  printf("%d\n", ans);

	return 0;
}

// O(4^n*min(n, m))