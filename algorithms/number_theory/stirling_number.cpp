#include <cstdio>
#include <vector>

#define ll long long

const ll MOD = 1000000007;
#define M(expr) ((expr) % MOD + MOD) % MOD


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

ll C_n_k(const std::vector<ll> &facts, ll n, ll k, ll mod) {
    ll ret = M( facts[n] * reprocical(facts[n - k], mod) );
    ret    = M( ret      * reprocical(facts[k    ], mod) );
    return ret;
}

ll StirlingNumber(const std::vector<ll> &facts, ll n, ll k, ll mod) {
    ll res = 0;

    for (int i = 0; i < k; ++i) {
        ll delta = M( C_n_k(facts, k, i, mod) * binpow(k - i, n, mod) ) * ((i & 1) ? -1 : +1);
        res += delta;
    }

    res = M(res);
    return M( res * reprocical(facts[k], mod) );
}

void cnt_factotials(std::vector<ll> &facts, ll n) {
    facts.resize(n);
    facts[0] = 1;
    for (ll i = 1; i < (ll) facts.size(); ++i) {
        facts[i] = M(facts[i - 1] * i);
    }
}


int main() {
    ll n = 123456;
    ll k = 12345;
    ll mod = MOD;

    std::vector<ll> facts;
    cnt_factotials(facts, 1000000);

    printf("stirling(%lld, %lld) = %lld (mod %lld)\n", n, k, StirlingNumber(facts, n, k, mod), mod);

    return 0;
}
