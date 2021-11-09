#include<stdio.h>
#include<math.h>

#define M 1000000
#define Mod 1000000000

int Lower[M+1], Higher[M+1];
char Used[M+1];

long long PrimePi(long long n)
{
    long long v = sqrt(n), p, temp, q, j, end, i, d, t;
    Higher[1] = n % Mod - 1;
    for(p = 2; p <= v; p++) {
        Lower[p] = p - 1;
        Higher[p] = (n/p) % Mod - 1;
    }
    for(p = 2; p <= v; p++) {
        if(Lower[p] == Lower[p-1])
            continue;
        temp = Lower[p-1];
        q = p * p;
        Higher[1] -= Higher[p] - temp;
        if(Higher[1] < 0)
            Higher[1] += Mod;
        j = 1 + (p & 1);
        end = (v <= n/q) ? v : n/q;
        for(i = p + j; i <= 1 + end; i += j) {
            if(Used[i])
                continue;
            d = i * p;
            if(d <= v)
                Higher[i] -= Higher[d] - temp;
            else {
                t = n/d;
                Higher[i] -= Lower[t] - temp;
            }
            if(Higher[i] < 0)
                Higher[i] += Mod;
        }
        if(q <= v)
            for(i = q; i <= end; i += p*j)
                Used[i] = 1;
        for(i = v; i >= q; i--) {
            t = i/p;
            Lower[i] -= Lower[t] - temp;
        }
    }
    return Higher[1] % Mod;
}

int main() {
    long long n = 0;
    scanf("%lld", &n);
    printf("%lld", PrimePi(n));
    return 0;
}
