#if 0
Необходимо обеспечить следующие абстракции:

stack * stack_new(size_t size);
stack_code stack_delete(stack *st)
stack_code stack_push(stack *st, int value);
stack_code stack_pop(stack *st, int *value);
Здесь typedef enum stack_code_e { STACK_OK, STACK_NULL, STACK_OVERFLOW, STACK_UNDERFLOW } stack_code; Этот перечислимый тип уже задан и вам его переопределять нельзя.
Вы должны написать структуру, которая будет использоваться в написанных вами четырёх функциях.

Должно быть предусмотрено, что может создаваться и уничтожаться произвольное количество стеков. Все неверные операции должны возвращать соответствующие коды ошибок.

Функцию main реализовать не нужно. Посланный вами файл должен содержать лишь определение вашей структуры и эти четыре функции.
#endif

typedef struct {
    size_t max_size;
    size_t size;
    int *buffer;
} stack;

stack *stack_new(size_t size) {
    if (size < 0) {
        return NULL;
    }

    stack *new_stack = calloc(1, sizeof(stack));
    new_stack->buffer = calloc(size + 2, sizeof(int));
    new_stack->max_size = size + 1;
    new_stack->size = 0;
    return new_stack;
}

stack_code stack_delete(stack *st) {
    if (st == NULL) {
        return STACK_NULL;
    }

    free(st->buffer);
    st->size = 0;
    free(st);

    return STACK_OK;
}

stack_code stack_push(stack *st, int value) {
    if (st == NULL) {
        return STACK_NULL;
    }
    if (st->size == st->max_size - 1) {
        return STACK_OVERFLOW;
    }

    st->size++;
    st->buffer[st->size] = value;
    return STACK_OK;
}

stack_code stack_pop(stack *st, int *value) {
    if (st == NULL) {
        return STACK_NULL;
    }
    if (st->size == 0) {
        return STACK_UNDERFLOW;
    }

    *value = st->buffer[st->size];
    st->buffer[st->size] = 0;
    st->size--;
    return STACK_OK;
}

