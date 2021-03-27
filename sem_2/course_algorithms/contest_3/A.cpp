/*
Вершина графа u называется инцидентной ребру e, если u является одним из концов ребра e.

Аналогично, ребро e называется инцидентным вершине u, если один из концов e — это вершина u.

Матрицей инцидентности графа G=(V,E) называется прямоугольная таблица из |V| строк и |E| столбцов, в которой на пересечении i-й строки и j-го столбца записана единица, если вершина i инцидентна ребру j, и ноль в противном случае.

Дан неориентированный граф. Выведите его матрицу инцидентности.
*/

#include <cstdio>
#include <cstdlib>
#include <vector>

using std::vector;

template<typename T>
using Row = vector<T>;

template<typename T>
using MatrixMap = vector<Row<T>>;

int main() {
	int n, m;
	scanf("%d %d", &n, &m);

	MatrixMap<bool> arr(n, Row<bool>(m));
	for (int i = 0; i < m; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		--a;
		--b;
		arr[a][i] = 1;
		arr[b][i] = 1;
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			printf("%d ", arr[i][j] ? 1 : 0);
		}
		printf("\n");
	}

	return 0;
}