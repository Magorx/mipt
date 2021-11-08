#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>


#define ll long long


const ll MOD = 1000000007;
#define M(expr) ((expr) % MOD + MOD) % MOD


#ifndef M_PI
#define M_PI 3.14159265359
#endif


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

ll C_n_k(const std::vector<ll> &facts, ll n, ll k, ll mod) {
    ll ret = M( facts[n] * get_reprocical(facts[n - k], mod) );
    ret    = M( ret      * get_reprocical(facts[k    ], mod) );
    return ret;
}

ll StirlingNumber(const std::vector<ll> &facts, ll n, ll k, ll mod) {
    ll res = 0;

    for (int i = 0; i < k; ++i) {
        ll delta = M( C_n_k(facts, k, i, mod) * binpow(k - i, n, mod) ) * ((i & 1) ? -1 : +1);
        res += delta;
    }

    res = M(res);
    return M( res * get_reprocical(facts[k], mod) );
}

int main() {
    int n = 0, k = 0;
	scanf("%d %d", &n, &k);

    std::vector<ll> arr(n);
	for (int i = 0; i < n; i++) {
		scanf("%lld", &arr[i]);
    }

    std::vector<ll> facts(n + 10, 1);
    facts[0] = 1;
    for (ll i = 1; i < (ll) facts.size(); ++i) {
        facts[i] = M(facts[i - 1] * i);
    }

    ll sum_weight = 0;
    for (auto w : arr) {
        sum_weight += w;
    }

    ll ans = M( M(sum_weight) * M( StirlingNumber(facts, n, k, MOD) + M( (n - 1) * StirlingNumber(facts, n - 1, k, MOD) ) ) );

    printf("%lld\n", ans);

	return 0;
}
