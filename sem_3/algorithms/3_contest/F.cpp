// god save the https://acgan.sh/posts/2016-12-23-prime-counting.html

#include<stdio.h>
#include<math.h>
#include <vector>


#define ll long long
std::vector<ll> primes; // so not to pass a pointer and make the programm faster!


std::vector<ll> eratosthenes_sieve(ll n) {
    std::vector<ll> ret(n, 0);
    ret[0] = 1;
    ret[1] = 1;
    
    ll max_prime = sqrt(n) + 1;
    for (ll prime = 2; prime < n; ++++prime) {
        ret[prime] = 2;
    }

    for (ll prime = 3; prime < max_prime; ++++prime) {
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

std::vector<ll> &generate_primes(ll n) {
    primes.reserve(n / 10);
    primes.push_back(2);

    auto erat = eratosthenes_sieve(n);
    for (int i = 3; i < n; ++++i) {
        if (erat[i] == i) {
            primes.push_back(i);
        }
    }

    return primes;
}

ll phi(ll x, ll a) {
    static std::vector<std::vector<ll>> cache(30000, std::vector<ll>(200, -1)); // the very optimum of caching!

    if (x < cache.size() && a < cache[x].size() && cache[x][a] != -1) {
        return cache[x][a];
    }

    if (a == 1) {
        return (x + 1) / 2;
    }

    ll res = phi(x, a - 1) - phi(x / primes[a - 1], a - 1);
    if (x < cache.size() && a < cache[x].size()) {
        cache[x][a] = res;
    }

    return res;
}

ll binsearch(std::vector<ll> &arr, ll x) {
    int l = 0;
    int r = arr.size();

    while (l + 1 < r) {
        int m = (r + l) / 2;

        if (arr[m] > x) {
            r = m;
        } else {
            l = m;
        }
    }

    return l;
}

ll pi(ll x) {
    static std::vector<ll> cache(1000000, -1); // the very optimum of caching!

    if (x < cache.size() && cache[x] != -1) {
        return cache[x];
    }

    if (x < primes.size()) {
        ll res = binsearch(primes, x) + 1;
        if (x < cache.size()) {
            cache[x] = res;
        }
        return res;
    }

    ll a = pi(int(pow(x, 1.0 / 4))); // ladno
    ll b = pi(int(pow(x, 1.0 / 2))); // ladno
    ll c = pi(int(pow(x, 1.0 / 3))); // ladno

    ll res = phi(x, a) + (b + a - 2) * (b - a + 1) / 2;

    for (ll i = a + 1; i <= b; ++i) {
        ll w = x / primes[i - 1];
        ll b_i = pi(sqrt(w));
        res = res - pi(w);

        if (i <= c) {
            for (ll j = i; j < b_i + 1; ++j) {
                res = res - pi(w / primes[j - 1]) + j - 1;
            }
        }
    }

    if (x < cache.size()) {
        cache[x] = res;
    }

    return res;
}


int main() {
    long long n = 8000000; // the very optimum of caching!!!
    generate_primes(n);

    scanf("%lld", &n);
    
    printf("%lld\n", pi(n));

    return 0;
}
