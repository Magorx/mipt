#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <cctype>

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
    if ((a & 1) == 0 && (b & 1) == 0) {
        return 2;
    }

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

    ll ans = 1 >> cnt;
    return a * ans;
}


inline unsigned long long read_next_long_long(char **buffer) {
    char *c = *buffer;
    while (c && *c == ' ' || *c == '\n') ++c;
 
    unsigned long long l = 0;
    while (*c >= '0' && *c <= '9') {
        l = l * 10 + *c - '0';
        ++c;
    }
 
    *buffer = c;
    return l;
}


int main() {
    FILE *fin = stdin;
 
    fseek(fin, 0, SEEK_END);
    long input_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);
 
    char *buffer = (char*) calloc(input_size + 1, sizeof(char));
    char *input_init_pointer_for_free = buffer;
    fread(buffer, sizeof(char), input_size, fin);
    fclose(fin);
//---------------------------------------------------------------
    ll n = read_next_long_long(&buffer);

    ll a = read_next_long_long(&buffer);
    --n;
    ll ans = 0;
    while (n--) {
        ll b = read_next_long_long(&buffer);
        // printf("gcd(%lld, %lld) = %lld\n", a, b, gcd(a, b));
        ans += gcd(a, b) != 1;
        a = b;
    }

    printf("%lld\n", ans);
}
