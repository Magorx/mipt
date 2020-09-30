/*
Даны n нестрого возрастающих массивов Ai и m нестрого убывающих массивов Bj. Все массивы имеют одну и ту же длину l. Далее даны q запросов вида (i,j), ответ на запрос – такое k, что max(Aik,Bjk) минимален. Если таких k несколько, можно вернуть любое.

Входные данные
На первой строке числа n,m,l (1≤n,m≤900;1≤l≤3000). Следующие n строк содержат описания массивов Ai. Каждый массив описывается перечислением l элементов. Элементы массива – целые числа от 0 до 105−1. Далее число m и описание массивов Bj в таком же формате. Массивы и элементы внутри массива нумеруются с 1. На следюущей строке число запросов q (1≤q≤n⋅m). Следующие q строк содержат пары чисел i,j (1≤i≤n, 1≤j≤m).

Выходные данные
Выведите q чисел от 1 до l – ответы на запросы.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//<KCTF>[EVERYDAY] GENERAL.H ==================================================

const int KCTF_DEBUG_LEVEL = 1; ///< Just a mode for debugging

int           DEBUG_NUMBER = 1;   ///< Just an int for debugging
unsigned char DEBUG_LETTER = 'a'; ///< Just a char for debugging

#define DEBUG_NUMBER_PRINT() printf("[deb] %d [deb]\n", DEBUG_NUMBER++);
#define DEBUG_LETTER_PRINT() printf("[deb] %c [deb]\n", DEBUG_LETTER++);

#define DEBUG_NUMBER_INCR() DEBUG_NUMBER++;
#define DEBUG_LETTER_INCR() DEBUG_LETTER++;

#define DEBUG(LEVEL) if (LEVEL <= KCTF_DEBUG_LEVEL)

const int INT_P = 7777777; /// Poison int

///  Return codes
enum RETURN_CODES {
    ERROR_FILE_NOT_FOUND = -10,
    ERROR_BIG_FILE,
    ERROR_MALLOC_FAIL,
    ERROR_NULL_OBJECT,
    ERROR_NO_RET_CODE,
    ERROR_BAD_ARGS,
    NULL_OBJ_OK = 0,
    RET_OK = 0,
};

//=============================================================================

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int binsearch_for_minmax(int *a, int *b, int length) {
    int l = 0;
    int r = length - 1;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        int cur_d = a[m] - b[m];
        if (cur_d == 0) {
            return m + 1;
        }

        if (cur_d < 0) {
            l = m;
        } else {
            r = m;
        }
    }

    if (max(a[l], b[l]) < max(a[r], b[r])) {
        return l + 1;
    } else {
        return r + 1;
    }
}

int main() {
    int n = 0;
    int m = 0;
    int l = 0;
    scanf("%d %d %d", &n, &m, &l);

    int **a = (int**) calloc(n, sizeof(int*));
    int **b = (int**) calloc(m, sizeof(int*));

    for (int i = 0; i < n; ++i) {
        a[i] = (int*) calloc(l, sizeof(int));
        for (int j = 0; j < l; ++j) {
            scanf("%d", &a[i][j]);
        }
    }

    for (int i = 0; i < m; ++i) {
        b[i] = (int*) calloc(l, sizeof(int));
        for (int j = 0; j < l; ++j) {
            scanf("%d", &b[i][j]);
        }
    }

    int q = 0;
    scanf("%d", &q);

    for (int z = 0; z < q; ++z) {
        int a_i = 0;
        int b_j = 0;
        scanf("%d %d", &a_i, &b_j);
        printf("%d\n", binsearch_for_minmax(a[a_i - 1], b[b_j - 1], l));
    }

    for (int i = 0; i < n; ++i) {
        free(a[i]);
    }
    free(a);

    for (int i = 0; i < m; ++i) {
        free(b[i]);
    }
    free(b);

    return 0;
}

// O(qlog(l))

