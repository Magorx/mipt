#include <vector>
#include <cmath>

#define ll long long

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

int main() {


    return 0;
}