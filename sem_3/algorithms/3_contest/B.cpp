#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>


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

std::vector<ll> eratosthenes_sieve(ll n) {
    std::vector<ll> ret(n, 0);
    ret[0] = -1;
    ret[1] = -1;
    
    ll max_prime = sqrt(n) + 1;
    for (ll prime = 2; prime < max_prime; ++prime) {
        if (ret[prime]) continue;

        ll p_i = prime;
        for (ll p_i = prime; p_i < n; p_i += prime) {
            if (ret[p_i]) continue;

            ret[p_i] = prime;
        }
    }

    return ret;
}

int main() {
    ll n;
    scanf("%lld", &n);

    auto ret = eratosthenes_sieve(n + 1);
    ll ans = 0;
    for (int i = 2; i <= n; ++i) {
        if (ret[i] < i) {
            ans += ret[i];
        }
    }

    printf("%lld\n", ans);

    return 0;
}
