#define __USE_MINGW_ANSI_STDIO 1

#include <stdlib.h>

#define STACK_SECURITY_LEVEL 10
#define STACK_DUMP_DEPTH 15

#define STACK_VALUE_TYPE int
#define STACK_VALUE_PRINTF_SPEC "%d"
#include "stack.h"
#undef STACK_VALUE_TYPE
#undef STACK_VALUE_PRINTF_SPEC

int main() {
    Stack_int s = {};
    VERIFY_OK(Stack_construct_int(&s));

    for (int i = 0; i < 1000; ++i) {
        VERIFY_OK(Stack_push_int(&s, rand()));
    }

    VERIFY_OK(Stack_dump_int(&s));

    for (int i = 0; i < 10000; ++i) {
        if ((Stack_pop_int(&s)) != OK)
            break;
    }

    s.capacity = 5;
    s.size = 10;
    VERIFY_OK(Stack_dump_int(&s));

    return 0;
}
