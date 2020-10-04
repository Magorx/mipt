/**
    \file
    \brief Stack with its full implementation
*/

#include <stdlib.h>
#include "general.h"

#ifndef STACK_VALUE_TYPE
#error STACK_VALUE_TYPE must be defined to define the stack properly
#endif // STACK_VALUE_TYPE

#ifndef STACK_VALUE_PRINTF_SPEC
#error STACK_VALUE_PRINTF_SPEC must be defined to dump the stack properly
#endif // STACK_VALUE_PRINTF_SPEC

#ifndef KCTF_STACK_CONSTANTS
#define KCTF_STACK_CONSTANTS

#define STACK_GENERIC(func) OVERLOAD(func, STACK_VALUE_TYPE)
#define STACK_OK(stack) do {CHECK_ERROR_ASSERT(STACK_GENERIC(Stack_valid)(stack) == OK);} while(0)
#define STACK_HASH(stack) STACK_GENERIC(Stack_hash)(stack)

enum stack_err{
    ERR_STACK_NOT_CREATED = -99,
    ERR_STACK_NOT_EXIST,
    ERR_BUFFER_NOT_EXIST,
    ERR_OVERFLOW,
    ERR_REALLOC_FAILED,
    ERR_BAD_HASH,

    OK = 0,
};

const STACK_VALUE_TYPE SVT_P = -7777777.0;

const size_t STACK_DUMP_DEPTH = 10;

const double STACK_REALLOC_UP_COEF = 1.5;
const double STACK_REALLOC_DOWN_COEF = 2;
#endif // KCTF_STACK_CONSTANTS

struct STACK_GENERIC(Stack_t) {
    long long full_hash;
    size_t capacity;
    size_t size;
    STACK_VALUE_TYPE *buffer;
};

typedef struct STACK_GENERIC(Stack_t) STACK_GENERIC(Stack);

long long STACK_GENERIC(Stack_hash)(const STACK_GENERIC(Stack) *cake) {
    CHECK_ERROR_ASSERT(cake);
    CHECK_ERROR_ASSERT(cake->buffer);
    return do_hash((char*)cake + sizeof(long long), sizeof(STACK_GENERIC(Stack)) - sizeof(long long)) + do_hash(cake->buffer, cake->capacity * sizeof(STACK_VALUE_TYPE));
}

int STACK_GENERIC(Stack_valid)(const STACK_GENERIC(Stack) *cake) {
    if (!cake) {
        RETURN_ERROR_ASSERT(ERR_STACK_NOT_EXIST);
    }

    if (!cake->buffer) {
        RETURN_ERROR_ASSERT(ERR_BUFFER_NOT_EXIST);
    }

    if (cake->size > cake->capacity) {
        RETURN_ERROR_ASSERT(ERR_OVERFLOW);
    }

    if (cake->full_hash != STACK_HASH(cake)) {
        RETURN_ERROR_ASSERT(ERR_BAD_HASH);
    }

    return OK;
}

int STACK_GENERIC(Stack_construct)(STACK_GENERIC(Stack) *cake) {
    CHECK_ERROR_ASSERT(cake != NULL);
    const int capacity = 32;
    cake->buffer = (STACK_VALUE_TYPE*) calloc(capacity, sizeof(STACK_VALUE_TYPE));
    if (!cake->buffer) {
        return ERR_STACK_NOT_CREATED;
    }

    cake->capacity = capacity;
    cake->size = 0;
    cake->full_hash = STACK_HASH(cake);
    STACK_OK(cake);
    return OK;
}

int STACK_GENERIC(Stack_destruct)(STACK_GENERIC(Stack) *cake) {
    STACK_OK(cake);

    free(cake->buffer);
    cake->capacity = SIZE_T_P;
    cake->size = SIZE_T_P;

    return OK;
}

size_t STACK_GENERIC(Stack_size)(const STACK_GENERIC(Stack) *s) {
    CHECK_ERROR_ASSERT(STACK_GENERIC(Stack_valid)(s) == OK);
    return s->size;
}

size_t STACK_GENERIC(Stack_capacity)(const STACK_GENERIC(Stack) *s) {
    CHECK_ERROR_ASSERT(STACK_GENERIC(Stack_valid)(s) == OK);
    return s->capacity;
;}

int STACK_GENERIC(Stack_dump)(const STACK_GENERIC(Stack) *cake) {
    const int validity = STACK_GENERIC(Stack_valid)(cake);
    printf("[DMP]<stack>: [ptr](%p) [valid](%s)\n", (void*) cake, validity ? "FALSE" : "true");
    printf("[   ]<     >: [size](%zu) [capacity](%zu)\n", STACK_GENERIC(Stack_size)(cake), STACK_GENERIC(Stack_capacity)(cake));
    printf("[   ]<     >: [buffer](%p)\n", (void*) (cake->buffer));

    const size_t print_depth = min(cake->size, STACK_DUMP_DEPTH);
    for (size_t i = 0; i < print_depth; ++i) {
        printf("[   ]<%3zu  >: [](" STACK_VALUE_PRINTF_SPEC ")\n", print_depth - 1 - i, cake->buffer[cake->size - 1 - i]);
    }

    return OK;
}

int STACK_GENERIC(Stack_realloc)(STACK_GENERIC(Stack) *cake, const double coef) {
    STACK_OK(cake);

    const size_t new_capacity = (double)cake->capacity * coef;
    if (new_capacity < cake->size) {
         RETURN_ERROR_ASSERT(ERR_REALLOC_FAILED);
    }

    STACK_VALUE_TYPE *new_buffer = (STACK_VALUE_TYPE*) realloc(cake->buffer, sizeof(STACK_VALUE_TYPE) * new_capacity);
    if (!new_buffer) {
        RETURN_ERROR_ASSERT(ERR_REALLOC_FAILED);
    }

    cake->buffer   = new_buffer;
    cake->capacity = new_capacity;
    cake->full_hash = STACK_HASH(cake);
    STACK_OK(cake);

    return OK;
}

int STACK_GENERIC(Stack_push)(STACK_GENERIC(Stack) *cake, const STACK_VALUE_TYPE val) {
    STACK_OK(cake);

    if (cake->size == cake->capacity - 1) {
        CHECK_ERROR_ASSERT(STACK_GENERIC(Stack_realloc)(cake, STACK_REALLOC_UP_COEF) == OK);
    }

    cake->buffer[cake->size++] = val;
    cake->full_hash = STACK_HASH(cake);
    STACK_OK(cake);

    return OK;
}

int STACK_GENERIC(Stack_pop)(STACK_GENERIC(Stack) *cake) {
    STACK_OK(cake);
    CHECK_ERROR_ASSERT(cake->size > 0);

    cake->buffer[--cake->size] = SVT_P;
    cake->full_hash = STACK_HASH(cake);

    if (cake->capacity / (cake->size + 1) > STACK_REALLOC_DOWN_COEF) {
        CHECK_ERROR_ASSERT(STACK_GENERIC(Stack_realloc)(cake, 1.0 / STACK_REALLOC_DOWN_COEF * 2) == 0);
    }

    cake->full_hash = STACK_HASH(cake);
    STACK_OK(cake);

    return OK;
}

/*int Stack_clear(STACK_GENERIC(Stack) *cake) {
    STACK_OK(cake);

    for (int i = 0; i < cake.size(); ++i) {
        CHECK_ERROR_ASSERT(STACK_GENERIC(Stack_pop)(cake) == OK);
    }

    return OK;
}

int Stack_is_empty(const STACK_GENERIC(Stack) *cake) {
    STACK_OK(cake);

    return cake->size == 0;
}

int Stack_is_empty(const STACK_GENERIC(Stack) *cake) {
    STACK_OK(cake);

    return cake->size == cake->capacity;
}*/
