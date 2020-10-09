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
    printf("[   ]<    >: [size](%d)\n", cake->size);
    printf("[   ]<    >: [capacity](%d)\n", cake->size);
    printf("[   ]<.buf>: \n");

    for (int i = 0; i < cake->capacity; ++i) {
        printf("[   ]<%3d  >: [](%d) -> (%d) (%d)\n", i, cake->buffer[i], i * 2, i * 2 + 1);
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

int main() {
    int n = 0;
    scanf("%d", &n);
    Heap h = {};
    Heap_construct(&h, n, -1);

    long long ret = 0;
    for (int i = 0; i < n; ++i) {
        long long x = 0;
        scanf("%d", &x);
        x *= 100;
        Heap_push(&h, x);
    }

    //Heap_dump(&h);

    for (int i = 0; i < n - 1; ++i) {
        long long a = Heap_top(&h);
        Heap_pop(&h);
        long long b = Heap_top(&h);
        Heap_pop(&h);

        ret += (a + b) / 100;
        Heap_push(&h, a + b);
    }

    printf("%g\n", (double) ret / 100);

    Heap_destruct(&h);

    return 0;
}
