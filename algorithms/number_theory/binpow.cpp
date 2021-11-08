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

int main() {
    ll x = 1234567890;
    ll p = 9876543210;
    ll m = 10000007;
    printf("pow(%lld, %lld) = %lld (mod %lld)\n", x, p, binpow(x, p, m), m);

    return 0;
}