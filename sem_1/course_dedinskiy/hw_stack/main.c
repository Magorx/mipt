#define __USE_MINGW_ANSI_STDIO 1

#include <stdlib.h>

#define VERIFY_BOMB

#define STACK_VALUE_TYPE int
#define STACK_VALUE_PRINTF_SPEC "%d"
#include "stack.h"
#undef STACK_VALUE_TYPE
#undef STACK_VALUE_PRINTF_SPEC

#define STACK_VALUE_TYPE double
#define STACK_VALUE_PRINTF_SPEC "%g"
#include "stack.h"
#undef STACK_VALUE_TYPE
#undef STACK_VALUE_PRINTF_SPEC

int main() {
    Stack_int s = {};
    VERIFY(Stack_construct_int(&s) == OK);

    for (int i = 0; i < 1000; ++i) {
        VERIFY(Stack_push_int(&s, rand()) == OK);
    }

    VERIFY(Stack_clear_int(&s) == OK);

    Stack_dump_int(&s);

    printf("==========================================================================\n");
    printf("===\nDOUBLES\n===\n");
    printf("==========================================================================\n");

    // USAGE EXAMPLE ==========================================================;
    Stack_double ss = {};
    VERIFY(Stack_construct_double(&ss) == OK);

    for (int i = 0; i < 1000; ++i) {
        VERIFY(Stack_push_double(&ss, (double) rand() / rand()) == OK);
    }

    Stack_dump_double(&ss);

    for (int i = 0; i < 10000; ++i) {
        if (rand() % 10 != 0) {
            VERIFY(Stack_pop_double(&ss) == OK);
        }
    }

    Stack_destruct_int(&s);
    Stack_destruct_double(&ss);

    return 0;
}

