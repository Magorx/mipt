/*
Дана очень длинная последовательность целых чисел длины N. Требуется вывести в отсортированном виде её наименьшие K элементов. Последовательность может не помещаться в память. Время работы O(N⋅logK), память O(K).
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define HEAP_TYPE long long

void swap(HEAP_TYPE *a, HEAP_TYPE *b) {
    HEAP_TYPE tmp = *b;
    *b = *a;
    *a = tmp;
}

typedef struct Heap_t {
    size_t size;
    size_t capacity;
    HEAP_TYPE *buffer;
    int type;
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

    cake->buffer = calloc(cap, sizeof(HEAP_TYPE));
    assert(cake->buffer);

    cake->size = 1;
    cake->capacity = cap;
    cake->type = type;
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
        swap(&cake->buffer[idx], &cake->buffer[idx / 2]);
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
                    swap(&cake->buffer[idx2], &cake->buffer[idx]);
                    Heap_sift_down(cake, idx2);
                }
            } else {
                if (val < val1) {
                    swap(&cake->buffer[idx1], &cake->buffer[idx]);
                    Heap_sift_down(cake, idx1);
                }
            }
        } else {
            if (val < val1) {
                swap(&cake->buffer[idx1], &cake->buffer[idx]);
                Heap_sift_down(cake, idx1);
            }
        }
    }
}

void Heap_push(Heap *cake, const HEAP_TYPE val) {
    assert(cake);

    cake->buffer[cake->size] = val * cake->type;
    Heap_sift_up(cake, cake->size);
    ++cake->size;
}

void Heap_pop(Heap *cake) {
    assert(cake);

    --cake->size;
    cake->buffer[1] = cake->buffer[cake->size];
    Heap_sift_down(cake, 1);
}

HEAP_TYPE Heap_top(Heap *cake) {
    assert(cake);
    return cake->buffer[1] * cake->type;
}


// Будем хранить k наименьших элементов последовательности, ведь это как раз то, что нас просят делать
// Каждый раз пуша элементы и проверяя, не много ли их в куче, получим то, что и хотим
int main() {
    int n = 0;
    int k = 0;
    scanf("%d %d", &n, &k);
    
    Heap h = {};
    Heap_construct(&h, k + 2, +1);

    for (int i = 0; i < n; ++i) {
        long long x = 0;
        scanf("%lld", &x);
        Heap_push(&h, x);
        if (Heap_size(&h) > k) {
            Heap_pop(&h);
        }
    }


    long long *arr = calloc(k + 1, sizeof(long long));
    while (Heap_size(&h)) {
        //printf("%d) ", h.size - 1);
        arr[h.size - 1] = Heap_top(&h);
        //printf("%lld\n", arr[h.size]);

        Heap_pop(&h);
    }

    for (int i = 1; i < k + 1; ++i) {
        printf("%lld ", arr[i]);
    }
    printf("\n");

    Heap_destruct(&h);

    return 0;
}

//O(NlogK) - N запросов и работа с кучей не более чем на K элементов