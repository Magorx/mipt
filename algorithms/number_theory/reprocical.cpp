#include <cstdio>

#define ll long long

template <typename T>
inline ll binpow(T x, ll p, ll mod) {
    if (!p) return 1;
    x %= mod;

    ll ret = 1;
    while (p > 0) {
        if (p & 1) {
            ret = ret * x % mod;
        }

        x = x * x % mod;
        p >>= 1;
    }

    return ret % mod;
}

template <typename T>
ll reprocical(T x, ll p) {
    return binpow(x, p - 2, p);
}

int main() {
    ll x = 1234567890;
    ll p = 9876543210;
    ll m = 10000007;
    printf("reprocical(%lld, mod = %lld) = %lld\n", x, m, reprocical(x, m));

    return 0;
}
