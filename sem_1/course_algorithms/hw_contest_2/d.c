/*
Реализуйте двоичную кучу.

Обработайте запросы следующих видов:

insert x: вставить целое число x в кучу;
getMin: вывести значение минимального элемента в куче (гарантируется, что к этому моменту куча не пуста);
extractMin: удалить минимальный элемент из кучи (гарантируется, что к этому моменту куча не пуста);
decreaseKey i Δ: уменьшить число, вставленное на i-м запросе, на целое число Δ≥0 (гарантируется, что i-й запрос был осуществлён ранее, являлся запросом добавления, а добавленное на этом шаге число всё ещё лежит в куче).
Можете считать, что в любой момент времени все числа, хранящиеся в куче, попарно различны, а их количество не превышает 105.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define HEAP_TYPE long long

void swap_h(HEAP_TYPE *a, HEAP_TYPE *b) {
    HEAP_TYPE tmp = *b;
    *b = *a;
    *a = tmp;
}

void swap_int(int *a, int *b) {
    int tmp = *b;
    *b = *a;
    *a = tmp;
}

typedef struct Heap_t {
    size_t size;
    size_t capacity;
    HEAP_TYPE *buffer;
    int type;
    int *f_indexes; // stores internal indexes of nodes by their external indexes
    int *b_indexes; // stores external indexes of nodes by their internal indexes
} Heap;

void Heap_dump(Heap *cake) {
    assert(cake);

    printf("[DMP]<heap>\n");
    printf("[   ]<    >: [size](%zu)\n", cake->size);
    printf("[   ]<    >: [capacity](%zu)\n", cake->capacity);
    printf("[   ]<.buf>: \n");

    for (int i = 0; i < cake->capacity; ++i) {
        printf("[   ]<%3d  >: [](%lld) -> (%d) (%d)\n", i, cake->buffer[i], i * 2, i * 2 + 1);
    }
}

void Heap_construct(Heap *cake, const size_t elem_cnt, int type) {
    size_t cap = elem_cnt + 1;

    cake->buffer = (HEAP_TYPE*) calloc(cap, sizeof(HEAP_TYPE));
    assert(cake->buffer);
    cake->f_indexes = (int*) calloc(cap, sizeof(int));
    assert(cake->f_indexes);
    cake->b_indexes = (int*) calloc(cap, sizeof(int));
    assert(cake->b_indexes);

    cake->size = 1;
    cake->capacity = cap;
    cake->type = type;
}

void Heap_swap(Heap *cake, int idx1, int idx2) {
    swap_h(&cake->buffer[idx1], &cake->buffer[idx2]); // swap values
    swap_int(&cake->f_indexes[cake->b_indexes[idx1]], &cake->f_indexes[cake->b_indexes[idx2]]); // swap ex->in indexes
    swap_int(&cake->b_indexes[idx1], &cake->b_indexes[idx2]); // swap in->ex indexes
}

size_t Heap_size(Heap *cake) {
    return cake->size - 1;
}

void Heap_destruct(Heap *cake) {
    assert(cake);
    free(cake->buffer);
}

void Heap_sift_up(Heap *cake, const size_t idx) {
    assert(cake);
    if (idx > cake->size || idx == 1) {
        return;
    }

    const HEAP_TYPE val = cake->buffer[idx];
    const HEAP_TYPE par = cake->buffer[idx / 2];
    if (val > par) {
        Heap_swap(cake, idx, idx / 2);
        Heap_sift_up(cake, idx / 2);
    }
}

void Heap_sift_down(Heap *cake, const size_t idx) {
    assert(cake);

    const size_t idx1 = idx * 2;
    const size_t idx2 = idx * 2 + 1;

    const HEAP_TYPE val = cake->buffer[idx];

    if (idx1 < cake->size) {
        const HEAP_TYPE val1 = cake->buffer[idx1];
        if (idx2 < cake->size) {
            const HEAP_TYPE val2 = cake->buffer[idx2];
            if (val1 < val2) {
                if (val < val2) {
                    Heap_swap(cake, idx2, idx);
                    Heap_sift_down(cake, idx2);
                }
            } else {
                if (val < val1) {
                    Heap_swap(cake, idx1, idx);
                    Heap_sift_down(cake, idx1);
                }
            }
        } else {
            if (val < val1) {
                Heap_swap(cake, idx1, idx);
                Heap_sift_down(cake, idx1);
            }
        }
    }
}

void Heap_push(Heap *cake, const HEAP_TYPE val, const int idx) {
    assert(cake);

    cake->buffer[cake->size] = val * cake->type;
    cake->f_indexes[idx] = cake->size;
    cake->b_indexes[cake->size] = idx;
    Heap_sift_up(cake, cake->size);
    ++cake->size;
}

void Heap_pop(Heap *cake) {
    assert(cake);

    --cake->size;
    Heap_swap(cake, 1, cake->size);
    Heap_sift_down(cake, 1);
}

HEAP_TYPE Heap_top(Heap *cake) {
    assert(cake);
    return cake->buffer[1] * cake->type;
}

int Heap_decrease(Heap *cake, int f_idx, HEAP_TYPE delta) {
    int b_idx = cake->f_indexes[f_idx]; // get inner index of 'node' by outer
    cake->buffer[b_idx] -= delta; // change the node
    Heap_sift_up(cake, b_idx); // sift it up

    return 0;
}

int main() {
    int n = 0;
    scanf("%d", &n);
    
    Heap h = {};
    Heap_construct(&h, n + 5, -1);

    char str[20] = {};
    for (int i = 1; i <= n; ++i) {
        scanf("%19s", str);
        if (strcmp(str, "insert") == 0) {
            long long val;
            scanf("%lld", &val);
            Heap_push(&h, val, i);
            //printf("ok\n");
        } else if (strcmp(str, "getMin") == 0) {
            printf("%lld\n", Heap_top(&h));
        } else if (strcmp(str, "extractMin") == 0) {
            Heap_pop(&h);
        } else if (strcmp(str, "decreaseKey") == 0) {
            long long val;
            int idx;
            scanf("%d %lld", &idx, &val);
            Heap_decrease(&h, idx, -val);
        }
    }

    Heap_destruct(&h);

    return 0;
}

// O(nlogn) - используем кучу при n запросах вставления максимум