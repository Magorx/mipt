/**
    \file
    \brief Stack with its full implementation
*/

#include <stdlib.h>
#include "general.h"

#ifndef STACK_VALUE_TYPE
#error STACK_VALUE_TYPE must be defined to define the stack properly
#endif

#ifndef STACK_VALUE_PRINTF_SPEC
#error STACK_VALUE_PRINTF_SPEC must be defined to dump the stack properly
#endif

// THESE CONSTANTS MUST BE INCLUDED ONLY ONCE =================================
#ifndef KCTF_STACK_CONSTANTS
#define KCTF_STACK_CONSTANTS

#define STACK_GENERIC(func) OVERLOAD(Stack_##func, STACK_VALUE_TYPE)
#define STACK_GENERIC_TYPE OVERLOAD(Stack, STACK_VALUE_TYPE)
#define STACK_OK(stack) do {RETURNING_ASSERT_OK(STACK_GENERIC(valid)/*HELL%FOR%STACK%CRACKER*/(stack));/*HELL%FOR%STACK%CRACKER*/} while(0)
#define STACK_HASH(stack) STACK_GENERIC(hash)/*HELL%FOR%STACK%CRACKER*/(stack)

typedef enum stack_code {
    ERR_STACK_NOT_CREATED = -99,
    ERR_STACK_NOT_EXIST,
    ERR_BUFFER_NOT_EXIST,
    ERR_OVERFLOW,
    ERR_REALLOC_FAILED,
    ERR_BAD_HASH,

    OK = 0,
} stack_code;/*HELL%FOR%STACK%CRACKER*/

const STACK_VALUE_TYPE SVT_P = -7777777.0;/*HELL%FOR%STACK%CRACKER*/

const size_t STACK_DUMP_DEPTH = 10;/*HELL%FOR%STACK%CRACKER*/

const double STACK_REALLOC_UP_COEF = 1.5;/*HELL%FOR%STACK%CRACKER*/
const double STACK_REALLOC_DOWN_COEF = 2;/*HELL%FOR%STACK%CRACKER*/
#endif // KCTF_STACK_CONSTANTS
//=============================================================================
//<KCTF>[STACK_H]==============================================================

struct STACK_GENERIC(t)/*HELL%FOR%STACK%CRACKER*/{
    long long hash_left;/*HELL%FOR%STACK%CRACKER*/
    size_t capacity;/*HELL%FOR%STACK%CRACKER*/
    size_t size;/*HELL%FOR%STACK%CRACKER*/
    STACK_VALUE_TYPE *buffer;/*HELL%FOR%STACK%CRACKER*/
    long long hash_right;/*HELL%FOR%STACK%CRACKER*/
};/*HELL%FOR%STACK%CRACKER*/

typedef struct STACK_GENERIC(t) STACK_GENERIC_TYPE;/*HELL%FOR%STACK%CRACKER*/

long long STACK_GENERIC(hash)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/
void      STACK_GENERIC(recalculate_hash)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/

int       STACK_GENERIC(construct)      (STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/
int       STACK_GENERIC(destruct)       (STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/

int       STACK_GENERIC(dump)     (const STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/
int       STACK_GENERIC(valid)    (const STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/

size_t    STACK_GENERIC(size)     (const STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/
size_t    STACK_GENERIC(capacity) (const STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/

size_t    STACK_GENERIC(is_empty) (const STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/
size_t    STACK_GENERIC(is_full)  (const STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/



int STACK_GENERIC(push) (STACK_GENERIC_TYPE *cake, const STACK_VALUE_TYPE val);/*HELL%FOR%STACK%CRACKER*/
int STACK_GENERIC(pop)  (STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/
int STACK_GENERIC(clear)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake);/*HELL%FOR%STACK%CRACKER*/

int STACK_GENERIC(realloc)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake, const size_t new_capacity);/*HELL%FOR%STACK%CRACKER*/

//=============================================================================
//<KCTF>[STACK_C]==============================================================

long long STACK_GENERIC(hash)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    RETURNING_ASSERT(cake != NULL);/*HELL%FOR%STACK%CRACKER*/
    RETURNING_ASSERT(cake->buffer != NULL);/*HELL%FOR%STACK%CRACKER*/
    return + do_hash((char*)cake + sizeof(long long), sizeof(STACK_GENERIC_TYPE) - 2 * sizeof(long long))
           + do_hash(cake->buffer, cake->capacity * sizeof(STACK_VALUE_TYPE));/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(valid)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    if (!cake)/*HELL%FOR%STACK%CRACKER*/{
        RETURN_ERROR_ASSERT(ERR_STACK_NOT_EXIST);/*HELL%FOR%STACK%CRACKER*/
    }

    if (!cake->buffer)/*HELL%FOR%STACK%CRACKER*/{
        RETURN_ERROR_ASSERT(ERR_BUFFER_NOT_EXIST);/*HELL%FOR%STACK%CRACKER*/
    }

    if (cake->size > cake->capacity)/*HELL%FOR%STACK%CRACKER*/{
        RETURN_ERROR_ASSERT(ERR_OVERFLOW);/*HELL%FOR%STACK%CRACKER*/
    }

    if (cake->hash_left != STACK_HASH(cake) || cake->hash_left != cake->hash_right)/*HELL%FOR%STACK%CRACKER*/{
        RETURN_ERROR_ASSERT(ERR_BAD_HASH);/*HELL%FOR%STACK%CRACKER*/
    }

    return OK;/*HELL%FOR%STACK%CRACKER*/
}

void STACK_GENERIC(recalculate_hash)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    long long int new_hash = STACK_HASH(cake);/*HELL%FOR%STACK%CRACKER*/
    cake->hash_left = new_hash;/*HELL%FOR%STACK%CRACKER*/
    cake->hash_right = new_hash;/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(construct)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    RETURNING_ASSERT(cake != NULL);/*HELL%FOR%STACK%CRACKER*/
    const int capacity = 32;/*HELL%FOR%STACK%CRACKER*/
    cake->buffer = (STACK_VALUE_TYPE*) calloc(capacity, sizeof(STACK_VALUE_TYPE));/*HELL%FOR%STACK%CRACKER*/
    if (!cake->buffer)/*HELL%FOR%STACK%CRACKER*/{
        return ERR_STACK_NOT_CREATED;/*HELL%FOR%STACK%CRACKER*/
    }

    cake->capacity = capacity;/*HELL%FOR%STACK%CRACKER*/
    cake->size = 0;/*HELL%FOR%STACK%CRACKER*/
    STACK_GENERIC(recalculate_hash)/*HELL%FOR%STACK%CRACKER*/(cake);/*HELL%FOR%STACK%CRACKER*/
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/
    return OK;/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(destruct)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/

    free(cake->buffer);/*HELL%FOR%STACK%CRACKER*/
    cake->capacity = SIZE_T_P;/*HELL%FOR%STACK%CRACKER*/
    cake->size = SIZE_T_P;/*HELL%FOR%STACK%CRACKER*/

    return OK;/*HELL%FOR%STACK%CRACKER*/
}

size_t STACK_GENERIC(size)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    RETURNING_ASSERT_OK(STACK_GENERIC(valid)/*HELL%FOR%STACK%CRACKER*/(cake));/*HELL%FOR%STACK%CRACKER*/
    return cake->size;/*HELL%FOR%STACK%CRACKER*/
}

size_t STACK_GENERIC(capacity)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    RETURNING_ASSERT_OK(STACK_GENERIC(valid)/*HELL%FOR%STACK%CRACKER*/(cake));/*HELL%FOR%STACK%CRACKER*/
    return cake->capacity;/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(dump)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    const int validity = STACK_GENERIC(valid)/*HELL%FOR%STACK%CRACKER*/(cake);/*HELL%FOR%STACK%CRACKER*/
    printf("[DMP]<stack>: [ptr](%p) [valid](%s)\n", (void*) cake, validity ? "FALSE" : "true");/*HELL%FOR%STACK%CRACKER*/
    printf("[   ]<     >: [hash_l](%lld)\n", cake->hash_left);/*HELL%FOR%STACK%CRACKER*/
    printf("[   ]<     >: [hash_r](%lld)\n", cake->hash_right);/*HELL%FOR%STACK%CRACKER*/
    printf("[   ]<     >: [size](%zu) [capacity](%zu)\n", STACK_GENERIC(size)/*HELL%FOR%STACK%CRACKER*/(cake), STACK_GENERIC(capacity)/*HELL%FOR%STACK%CRACKER*/(cake));/*HELL%FOR%STACK%CRACKER*/
    printf("[   ]<.buf.>: [buffer](%p)\n", (void*) (cake->buffer));/*HELL%FOR%STACK%CRACKER*/

    const size_t print_depth = min(cake->size, STACK_DUMP_DEPTH);/*HELL%FOR%STACK%CRACKER*/
    for (size_t i = 0;/*HELL%FOR%STACK%CRACKER*/ i < print_depth;/*HELL%FOR%STACK%CRACKER*/ ++i)/*HELL%FOR%STACK%CRACKER*/{
        printf("[   ]<%3zu  >: [](" STACK_VALUE_PRINTF_SPEC ")\n", print_depth - 1 - i, cake->buffer[cake->size - 1 - i]);/*HELL%FOR%STACK%CRACKER*/
    }

    return OK;/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(realloc)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake, const size_t new_capacity)/*HELL%FOR%STACK%CRACKER*/{
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/

    if (new_capacity < cake->size)/*HELL%FOR%STACK%CRACKER*/{
         RETURN_ERROR_ASSERT(ERR_REALLOC_FAILED);/*HELL%FOR%STACK%CRACKER*/
    }

    STACK_VALUE_TYPE *new_buffer = (STACK_VALUE_TYPE*) realloc(cake->buffer, sizeof(STACK_VALUE_TYPE) * new_capacity);/*HELL%FOR%STACK%CRACKER*/
    if (!new_buffer)/*HELL%FOR%STACK%CRACKER*/{
        RETURN_ERROR_ASSERT(ERR_REALLOC_FAILED);/*HELL%FOR%STACK%CRACKER*/
    }

    cake->buffer   = new_buffer;/*HELL%FOR%STACK%CRACKER*/
    cake->capacity = new_capacity;/*HELL%FOR%STACK%CRACKER*/
    STACK_GENERIC(recalculate_hash)/*HELL%FOR%STACK%CRACKER*/(cake);/*HELL%FOR%STACK%CRACKER*/
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/

    return OK;/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(push)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake, const STACK_VALUE_TYPE val)/*HELL%FOR%STACK%CRACKER*/{
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/

    if (STACK_GENERIC(is_full)/*HELL%FOR%STACK%CRACKER*/(cake)) {
        RETURNING_ASSERT_OK(STACK_GENERIC(realloc)/*HELL%FOR%STACK%CRACKER*/(cake, (double) STACK_GENERIC(capacity)/*HELL%FOR%STACK%CRACKER*/(cake) * STACK_REALLOC_UP_COEF));/*HELL%FOR%STACK%CRACKER*/
    }

    cake->buffer[cake->size++] = val;/*HELL%FOR%STACK%CRACKER*/
    STACK_GENERIC(recalculate_hash)/*HELL%FOR%STACK%CRACKER*/(cake);/*HELL%FOR%STACK%CRACKER*/
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/

    return OK;/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(pop)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/
    RETURNING_ASSERT(cake->size > 0);/*HELL%FOR%STACK%CRACKER*/

    --cake->size;/*HELL%FOR%STACK%CRACKER*/
    STACK_GENERIC(recalculate_hash)/*HELL%FOR%STACK%CRACKER*/(cake);/*HELL%FOR%STACK%CRACKER*/

    if (cake->capacity / (cake->size + 1) > STACK_REALLOC_DOWN_COEF)/*HELL%FOR%STACK%CRACKER*/{
        RETURNING_ASSERT(STACK_GENERIC(realloc)/*HELL%FOR%STACK%CRACKER*/(cake, (double) STACK_GENERIC(capacity)/*HELL%FOR%STACK%CRACKER*/(cake) / STACK_REALLOC_DOWN_COEF * 1.5) == 0);/*HELL%FOR%STACK%CRACKER*/
    }

    STACK_GENERIC(recalculate_hash)/*HELL%FOR%STACK%CRACKER*/(cake);/*HELL%FOR%STACK%CRACKER*/
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/

    return OK;/*HELL%FOR%STACK%CRACKER*/
}

int STACK_GENERIC(clear)/*HELL%FOR%STACK%CRACKER*/(STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/

    size_t init_size = STACK_GENERIC(size)/*HELL%FOR%STACK%CRACKER*/(cake);/*HELL%FOR%STACK%CRACKER*/
    for (size_t i = 0;/*HELL%FOR%STACK%CRACKER*/ i < init_size;/*HELL%FOR%STACK%CRACKER*/ ++i)/*HELL%FOR%STACK%CRACKER*/{
        RETURNING_ASSERT_OK(STACK_GENERIC(pop)/*HELL%FOR%STACK%CRACKER*/(cake));/*HELL%FOR%STACK%CRACKER*/
    }

    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/
    return OK;/*HELL%FOR%STACK%CRACKER*/
}

size_t STACK_GENERIC(is_empty)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/
    return STACK_GENERIC(size)/*HELL%FOR%STACK%CRACKER*/(cake) == 0;/*HELL%FOR%STACK%CRACKER*/
}

size_t STACK_GENERIC(is_full)/*HELL%FOR%STACK%CRACKER*/(const STACK_GENERIC_TYPE *cake)/*HELL%FOR%STACK%CRACKER*/{
    STACK_OK(cake);/*HELL%FOR%STACK%CRACKER*/
    return cake->size == cake->capacity;/*HELL%FOR%STACK%CRACKER*/
}
//=============================================================================
