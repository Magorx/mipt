#include <cstdlib>
#include <cstdio>
#include <cassert>


#define ull unsigned long long


const int P = 1000000007;


template <typename T>
T binpow(const T &obj, ull pow, ull mod) {
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


ull get_reprocical(ull x, ull p) {
    return binpow(x, p - 2, p);
}


int main() {
    const int N = 1000000;
    
    ull *fact   = (ull*) calloc(N, sizeof(ull));
    ull *unfact = (ull*) calloc(N, sizeof(ull));

    fact[0] = fact[1] = 1;
    unfact[0] = unfact[1] = 1;

    ull n, l, q;
    scanf("%llu %llu %llu", &n, &l, &q);

    for (int i = 2; i <= N; ++i) {
        fact[i] = (fact [i - 1] * i) % P;
    }

    unfact[N] = get_reprocical(fact[N], P);
    for (int i = N - 1; i >= 1; --i) {
        unfact[i] = (unfact[i + 1] * (i + 1)) % P;
    }

    for (int i = 0; i < 10; ++i) {
        printf("fact[%d] = %llu, unfact[%d] = %llu\n", i, fact[i], i, unfact[i]);
    }
    printf("inv(8) = %llu\n", get_reprocical(8, P));

    ull s = 0;
    for (int k = l, cnt = 0; cnt < q; k += l, ++cnt) {
        if (k > n) break;

        ull x = ((fact[n] * unfact[n - k]) % P) * unfact[k];
        s = (s + x) % P;
    }

    printf("%llu\n", s);

    return 0;
}
