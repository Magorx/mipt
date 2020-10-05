#define __USE_MINGW_ANSI_STDIO 1

#include <stdlib.h>

#define STACK_SECURITY_LEVEL 1
#define STACK_DUMP_DEPTH 15

#define STACK_VALUE_TYPE int
#define STACK_VALUE_PRINTF_SPEC "%d"
#include "stack.h"
#undef STACK_VALUE_TYPE
#undef STACK_VALUE_PRINTF_SPEC

int main() {
    Stack_int s = {};
    ASSERT_OK(Stack_construct_int(&s));

    for (int i = 0; i < 1000; ++i) {
        ASSERT_OK(Stack_push_int(&s, rand()));
    }

    ASSERT_OK(Stack_dump_int(&s));

    for (int i = 0; i < 10000; ++i) {
        if ((Stack_pop_int(&s)) != OK)
            break;
    }

    s.capacity = 5;
    s.size = 10;
    ASSERT_OK(Stack_dump_int(&s));

    return 0;
}

/* TODO
printf_formated(what, from_who) with statics

Unit tests

capacity overload

canary to general ??!?!??!?!!?!

Byte poison, dump, etc.

#ifdef XXX -> XXX + 0

ASSERT -> VERIFIE ~~~

#define ERRNAME(error, descr) -> sace error: return #error ": " descr
*/
