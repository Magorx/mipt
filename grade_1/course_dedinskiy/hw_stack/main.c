#include <stdlib.h>

#include "general.h"
#include "stack.h"

int is_zero(int a) {
    return a == 0;
}

int main() {
    Stack s = {};
    ASSERT(Stack_construct(&s) == OK);

    for (int i = 0; i < 1000; ++i) {
        Stack_push(&s, rand());
    }

    Stack_dump(&s);

    for (int i = 0; i < 10000; ++i) {
        if (rand() % 10 != 0) {
            Stack_pop(&s);
        }
    }


    return 0;
}

