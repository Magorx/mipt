#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <complex>
#include <iostream>


using std::cin;
using std::cout;


#define ll long long
#define cplx std::complex<double>

const ll MOD = 1000000007;
#define M(expr) ((expr) % MOD + MOD) % MOD


#ifndef M_PI
#define M_PI 3.14159265359
#endif


template <typename T>
T binpow(const T &obj, ll pow, ll mod) {
  if (pow == 1) {
    return obj % mod;
  } else {
    if (pow % 2) {
      return (binpow(obj, pow - 1, mod) * obj) % mod;
    } else {
      T tmp = binpow(obj, pow / 2, mod);
      return (tmp * tmp) % mod;
    }
  }
}

template <typename T>
ll get_reprocical(T x, ll p) {
    return binpow(x, p - 2, p);
}

inline ll cnt_zeroes(ll x) {
    ll cnt = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++cnt;
    }

    return cnt;
}

ll gcd(ll a, ll b) {
    const ll cnt_a = cnt_zeroes(a);
    const ll cnt_b = cnt_zeroes(b);
    const ll cnt = cnt_a > cnt_b ? cnt_b : cnt_a;

    a >>= cnt;
    b >>= cnt;
    while (a != b) {
        if (a > b) std::swap(a, b);
        b -= a;

        b >>= cnt_zeroes(b);
    }

    ll ans = 1 << cnt;
    return a * ans;
}

inline bool coprime(ll a, ll b) {
    return ((a & 1) == 0 && (b & 1) == 0) || gcd(a, b) == 1;
}

std::vector<ll> eratosthenes_sieve(ll n) {
    std::vector<ll> ret(n, 0);
    ret[0] = -1;
    ret[1] = -1;
    
    ll max_prime = sqrt(n) + 1;
    for (ll prime = 2; prime < max_prime; ++prime) {
        if (ret[prime]) continue;

        ll max_k = n / prime;
        for (ll k = 1; k <= max_k; ++k) {
            if (ret[prime * k]) continue;

            ret[prime * k] = prime;
        }
    }

    return ret;
}
 
void fft(std::vector<cplx> &coef, bool invert = false) {
	size_t coef_size = (size_t) coef.size();
 
	for (size_t i = 1, j = 0; i < coef_size; ++i) {
		size_t bit = coef_size >> 1;
		for (; j >= bit; bit = bit >> 1) { j -= bit; }

		j += bit;
		if (i < j) {
			std::swap(coef[i], coef[j]);
        }
	}
 
    double invert_sign = invert ? -1 : +1;
	for (size_t idx = 2; idx <= coef_size; idx = idx << 1) {
		double ang = invert_sign * 2 * M_PI / idx;

		cplx root_len(cos(ang), sin(ang));
		for (size_t i = 0; i < coef_size; i += idx) {
			cplx root = 1;
			for (size_t j = 0; j < idx / 2; ++j) {
				cplx a = coef[i + j];
                cplx b = root * coef[i + j + idx / 2];

				coef[i + j] = a + b;
				coef[i + j + idx / 2] = a - b;
				root *= root_len;
			}
		}
	}

	if (!invert) return;

    for (size_t i = 0; i < coef_size; ++i)
        coef[i] /= coef_size;
}

template <typename T1, typename T2>
void copy_vec(std::vector<T1> &a, const std::vector<T2> &b) {
    a.resize(b.size());
    for (size_t i = 0; i < b.size(); ++i) {
        a[i] = (T1) b[i];
    }
}

template <typename T>
double sign(const T &x) {
    if (x < 0) return -1;
    if (x > 0) return +1;
    return 0;
}

std::vector<ll> multiply_polynomials(const std::vector<ll> &a, const std::vector<ll> &b) {
	std::vector<cplx> fft_a;
    std::vector<cplx> fft_b;
    copy_vec(fft_a, a);
    copy_vec(fft_b, b);

    size_t max_size = std::max(fft_a.size(), fft_b.size());
	size_t fft_size = 1;
	while (fft_size < max_size) fft_size = fft_size << 1;
	fft_size = fft_size << 1;
	fft_a.resize(fft_size);
    fft_b.resize(fft_size);
 
	fft(fft_a);
    fft(fft_b);
	for (size_t i = 0; i < fft_size; ++i) {
		fft_a[i] *= fft_b[i];
    }
	fft(fft_a, true);
 
    std::vector<ll> ret(fft_size);
	for (size_t i = 0; i < fft_size; ++i) {
        auto res = fft_a[i].real();
		ret[i] = (ll)(sign(res) * 0.5 + res);
    }

    size_t last_first_zero = 0;
    bool flag = false;
    for (size_t i = 1; i < ret.size(); ++i) {
        if (!ret[i] && ret[i - 1]) {
            last_first_zero = i;
        }
    }
    ret.resize(last_first_zero);

    return ret;
}


int main() {
    int n, m;
    std::vector<ll> a;
    std::vector<ll> b;

    cin >> n;
    a.resize(n + 1, 0);
    for (int i = n; i >= 0; --i) {
        cin >> a[i];
    }

    cin >> m;
    b.resize(m + 1, 0);
    for (int i = m; i >= 0; --i) {
        cin >> b[i];
    }

    auto ret = multiply_polynomials(a, b);

    cout << std::max((int) ret.size() - 1, 0) << ' ';
    for (int i = (int) ret.size() - 1; i >= 0; --i) {
        cout << ret[i] << ' ';
    }
    cout << '\n';

    return 0;
}
