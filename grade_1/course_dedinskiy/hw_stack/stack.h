/**
    \file
    \brief Stack with its full implementation
*/

#ifndef KCTF_STACK_H
#define KCTF_STACK_H

#include <stdlib.h>
#include "general.h"

enum stack_err {
    ERR_STACK_NOT_CREATED = -99,
    ERR_STACK_NOT_EXIST,
    ERR_BUFFER_NOT_EXIST,
    ERR_OVERFLOW,
    ERR_REALLOC_FAILED,

    OK = 0,
};

#define STACK_VALUE_TYPE int
#define STACK_VALUE_PRINTF_SPEC "%d"

const STACK_VALUE_TYPE SVT_P = -7777777;

const size_t STACK_DUMP_DEPTH = 10;

const double STACK_REALLOC_UP_COEF = 1.5;
const double STACK_REALLOC_DOWN_COEF = 2;

struct Stack_t {
    size_t capacity;
    size_t size;
    STACK_VALUE_TYPE *buffer;
};

typedef struct Stack_t Stack;

int Stack_valid(const Stack *cake) {
    if (!cake) {
        RETURN_ERROR_ASSERT(ERR_STACK_NOT_EXIST);
    }

    if (!cake->buffer) {
        RETURN_ERROR_ASSERT(ERR_BUFFER_NOT_EXIST);
    }

    if (cake->size > cake->capacity) {
        RETURN_ERROR_ASSERT(ERR_OVERFLOW);
    }

    return OK;
}

int Stack_construct(Stack *cake) {
    ASSERT(cake);

    const int capacity = 32;
    cake->buffer = calloc(capacity, sizeof(STACK_VALUE_TYPE));
    if (!cake->buffer) {
        return ERR_STACK_NOT_CREATED;
    }

    cake->capacity = capacity;
    cake->size = 0;

    ASSERT(!Stack_valid(cake));

    return OK;
}

int Stack_destruct(Stack *cake) {
    ASSERT(!Stack_valid(cake));

    free(cake->buffer);
    cake->capacity = SIZE_T_P;
    cake->size = SIZE_T_P;

    return OK;
}

size_t Stack_size(Stack *s) {
    ASSERT(Stack_valid(s) == OK);
    return s->size;
}

size_t Stack_capacity(Stack *s) {
    ASSERT(Stack_valid(s) == OK);
    return s->capacity;
;}

int Stack_dump(const Stack *cake) {
    const int validity = Stack_valid(cake);
    printf("[DMP]<stack>: [ptr](%p) [valid](%s)\n", cake, validity ? "FALSE" : "true");
    printf("[   ]<     >: [size](%u) [capacity](%u)\n", Stack_size(cake), Stack_capacity(cake));
    printf("[   ]<     >: [buffer](%p)\n", cake->buffer);

    const size_t print_depth = min(cake->size, STACK_DUMP_DEPTH);
    for (size_t i = 0; i < print_depth; ++i) {
        printf("[   ]<%3d  >: [](" STACK_VALUE_PRINTF_SPEC ")\n", print_depth - 1 - i, cake->buffer[cake->size - 1 - i]);
    }

    return OK;
}

int Stack_realloc(Stack *cake, const double coef) {
    ASSERT(Stack_valid(cake) == OK);

    const size_t new_capacity = (double)cake->capacity * coef;
    if (new_capacity < cake->size) {
         RETURN_ERROR_ASSERT(ERR_REALLOC_FAILED);
    }

    STACK_VALUE_TYPE *new_buffer = realloc(cake->buffer, sizeof(STACK_VALUE_TYPE) * new_capacity);
    if (!new_buffer) {
        RETURN_ERROR_ASSERT(ERR_REALLOC_FAILED);
    }

    cake->buffer   = new_buffer;
    cake->capacity = new_capacity;

    return OK;
}

int Stack_push(Stack *cake, const STACK_VALUE_TYPE val) {
    ASSERT(Stack_valid(cake) == OK);

    if (cake->size == cake->capacity - 1) {
        ASSERT(Stack_realloc(cake, STACK_REALLOC_UP_COEF) == OK);
    }

    cake->buffer[cake->size++] = val;

    return OK;
}

int Stack_pop(Stack *cake) {
    ASSERT(Stack_valid(cake) == OK);
    ASSERT(cake->size > 0);

    cake->buffer[--cake->size] = SVT_P;

    if (cake->capacity / (cake->size + 1) > STACK_REALLOC_DOWN_COEF) {
        ASSERT(Stack_realloc(cake, 1.0 / STACK_REALLOC_DOWN_COEF * 2) == 0);
    }

    return OK;
}

int Stack_clear(Stack *cake) {
    ASSERT(Stack_valid(cake) == OK);

    for (int i = 0; i < cake.size(); ++i) {
        ASSERT(Stack_pop(cake) == OK);
    }

    return OK;
}

int Stack_is_empty(const Stack *cake) {
    ASSERT(Stack_valid(cake) == OK);

    return cake->size == 0;
}

int Stack_is_empty(const Stack *cake) {
    ASSERT(Stack_valid(cake) == OK);

    return cake->size == cake->capacity;
}

#endif // KCTF_STACK_H
