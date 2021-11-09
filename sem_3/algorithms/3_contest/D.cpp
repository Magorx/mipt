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
    ret[0] = 1;
    ret[1] = 1;
    
    ll max_prime = sqrt(n) + 1;
    for (ll prime = 2; prime < max_prime; ++prime) {
        if (ret[prime]) continue;

        ll p_i = prime;
        for (ll p_i = prime; p_i < n; p_i += prime) {
            if (ret[p_i]) continue;

            ret[p_i] = prime;
        }
    }

    for (int i = max_prime - 1; i < n; ++i) {
        if (!ret[i]) ret[i] = i;
    }

    return ret;
}

inline void mark_used(std::vector<bool> &used, const std::vector<ll> &erat, ll x) {
    while (x != 1) {
        used[erat[x]] = true;
        x /= erat[x];
    }
}

int main() {
    int n = 0;
    scanf("%d", &n);
    auto erat = eratosthenes_sieve(10000009);

    std::vector<ll> arr(n);
    for (int i = 0; i < n; i++) {
        scanf("%lld", &arr[i]);
    }

    std::vector<bool> used(erat.size() + 1);

    int idx = 0;
    while (idx < n) {
        bool flag = true; // if we can just use arr[idx]
        int x = arr[idx];

        while (x != 1) {
            if (used[erat[x]]) {
                flag = false;
                break;
            }

            x /= erat[x];
        }

        if (flag) {
            mark_used(used, erat, arr[idx++]);
            continue;
        }

        while (!flag) {
            flag = true;
            x = ++arr[idx];
            while (x != 1) {
                if (used[erat[x]]) {
                    flag = false;
                    break;
                }

                x /= erat[x];
            }
        }

        mark_used(used, erat, arr[idx++]);
        break;
    }
    
    int p = 2; // find any unused prime number, our posledovatelnost' is already bigger
    while (idx < n) {
        while (true) {
            if (used[p] || erat[p] != p) {
                p++;
                continue;
            }

            arr[idx++] = p++;
            break;
        }
    }
    
    for (int i = 0; i < n; i++) {
        printf("%lld ", arr[i]);
    }

    return 0;
}
