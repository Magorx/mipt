#include <cstdio>

#define ll long long

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
        if (a > b) { ll tmp = a; a = b; b = tmp; }
        b -= a;

        b >>= cnt_zeroes(b);
    }

    ll ans = 1 << cnt;
    return a * ans;
}

inline bool coprime(ll a, ll b) {
    return ((a & 1) == 0 && (b & 1) == 0) || gcd(a, b) == 1;
}

int main() {
    ll a = 12344424634;
    ll b = 2529340110;
    printf("gcd(%lld, %lld) = %lld\n", a, b, gcd(a, b));

    return 0;
}
