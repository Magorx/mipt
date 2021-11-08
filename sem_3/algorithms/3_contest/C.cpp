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

int main() {
    ll n;
    scanf("%lld", &n);

    std::vector<ll> arr(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &arr[i]);
    }


    int l = 0;
    int r = 1000000000;
    int ones = 0;

    for (int i = 0; i < n; ++i) {
        if (arr[i] == 1) {
            ++ones;
            continue;
        }

        ll cur = arr[i];
        // printf("--- i = %d\n", i);
        for (int j = i + 1; j < n; ++j) {
            // printf("j = %d | cur = %lld\n", j, cur);
            cur = gcd(cur, arr[j]);

            if (cur == 1) {
                if (j - i < r - l) {
                    r = j;
                    l = i;
                }
                continue;
            }
        }
    }

    if (r == 1000000000 && !ones) {
        printf("-1\n");
        return 0;
    }

    // printf("l %d r %d\n", l, r);

    if (ones) {
        int ans = n - ones;
        printf("%d\n", ans);
    } else {
        int ans = n + r - l - 1;
        printf("%d\n", ans);
    }

    return 0;
}
