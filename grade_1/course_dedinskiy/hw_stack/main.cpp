#define __USE_MINGW_ANSI_STDIO 1

#include <stdlib.h>

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

int is_zero(int a) {
    return a == 0;
}

int main() {
    Stack_int s = {};
    ASSERT_OK(Stack_construct_int(&s));

    for (int i = 0; i < 1000; ++i) {
        ASSERT_OK(Stack_push_int(&s, rand()));
    }

    ASSERT_OK(Stack_dump_int(&s));

    for (int i = 0; i < 10000; ++i) {
        ASSERT_OK(Stack_pop_int(&s));
    }

    return 0;
}

