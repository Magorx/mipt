#include <cstdio>
#include <bitset>
using std::bitset;
int main() {
	int n = 0, c;
	bitset<2001> g[n];
	for (int i = 0; i < n; ++i) {
		char c;
		for (int j = 0; j < n; ++j) {
			scanf("%c", &c);
			g[i][j] = c - '0';		
		}
		scanf("%c", &c);
	}
	for (int k = 0; k < n; ++k)
		for (int j = 0; j < n; ++j)
			g[j] |= g[j][k] ? g[k] : 0;
	for (int i = 0; i < n * n; ++i) {
		printf("%d", (int) g[i / n][i % n]);
		if ((i % n) == n - 1) printf("\n");
	}
	return 0;
}
