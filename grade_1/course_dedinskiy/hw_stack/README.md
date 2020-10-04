#Usage:

```
#include "general.h"

#define STACK_VALUE_TYPE int
#define STACK_VALUE_PRINTF_SPEC "%d"
#include "stack.h"
#undef STACK_VALUE_TYPE
#undef STACK_VALUE_PRINTF_SPEC

...main...
Stack_int s = {};
ASSERT_OK(Stack_construct_int(&s));
ASSERT_OK(Stack_push_int(&s, rand()));
ASSERT_OK(Stack_dump_int(&s));
ASSERT_OK(Stack_pop_int(&s));
```

Programm WILL NOT be terminated if any stack command fails without using
```
#define ASSERT_BOMB
```
BEFORE including "stack.h"

That's why I wraped every command in ASSERT_OK() in example. This way you can see whole traceback of fallen function.