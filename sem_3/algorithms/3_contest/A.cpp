#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>


#define ll long long


const ll MOD = 1000000007;


#define M(expr) ((expr) % MOD + MOD) % MOD


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
    if ((a & 1) == 0 && (b & 1) == 0) {
        return 2;
    }

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

    ll ans = 1 >> cnt;
    return a * ans;
}

int main() {
    ll a, b, c, d;
    scanf("%lld %lld %lld %lld", &a, &b, &c, &d);

    ll ans = M(M(a * d) + M(b * c));
    ll repr = M(get_reprocical(M(b * d), MOD));

    ans = M(ans * repr);

    printf("%lld\n", ans);

    return 0;
}
