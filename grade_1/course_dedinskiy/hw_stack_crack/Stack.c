// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
Copyright (c) 2020  MIPT
Module Name:
    Stack.h
Abstract:
    Реализует класс стека для 64-битных переменных, а также имплементацию
    его методов и собственных "системных" функций.
Author:
    JulesIMF
Last Edit:
    07.10.2020 22:51
Edit Notes:
    1) Добавлен stackCopy
    2) Добавлен stackRescue
*/
#pragma once
#include "Stack.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <intrin.h>
#include <string.h>
#pragma intrinsic(_ReturnAddress)

//Макрос для создания копии
#define DUPLICATE(STACK, COPY)                                  \
Stack* COPY = makeStackDuplicate(STACK);                        \
if (COPY == NULL)                                               \
    return STACK_DUPLICATION_ERROR;                             \
COPY += duplicateOffset;                                        \
stackDelete(STACK->duplicate - duplicateOffset);                \
STACK->duplicate = COPY;

//Макрос для возврата STACK_INVALID
#define CHECK(EXPRESSION)                                       \
if(EXPRESSION)                                                  \
{                                                               \
    if(_STACK_PRINT_MESSAGES)                                   \
        printf("Check failed in %s: \"%s\", line %d\n",         \
               __FUNCTION__, #EXPRESSION, __LINE__);            \
    return STACK_INVALID;                                       \
}

#define STATIC_VALUES() if(!frontCanary) { if(!strcmp(__FUNCTION__, "stackNew")) generateValues(); else return STACK_INVALID; }

#define _STACK_DUPLICATE_OFFSET
#define _STACK_HASH
#define _STACK_RETURN_ADDRESS
#define _STACK_PRINT_MESSAGES 1
#define _STACK_DUPLICATE
#define _STACK_DEEP_VALIDATION




static unsigned long long frontCanary   = 0, backCanary = 0, poison = 0;
static int const maxAbsOffset           = 10;
static int duplicateOffset              = 0;

/**
 * Генерирует системные значения.
 * \warning СИСТЕМНАЯ ФУНКЦИЯ
 */
static void generateValues(void)
{
#ifdef    _STACK_RETURN_ADDRESS
    static int timesCalled = 0;

    timesCalled++;

    if (timesCalled > 1)
    {
    #ifdef    _STACK_PRINT_MESSAGES
        printf("\tFunction %s was unexpectedly called from %p.\n\n", __FUNCTION__);
    #endif //!_STACK_PRINT_MESSAGES
        return;
    }
#endif //!_STACK_RETURN_ADDRESS

    srand(time(0)); //Прости, дорогой пользователь, я не нашел способа сохранить предыдущую начальную точку (((
    while (!frontCanary)
    {
        //Запихаем рандомные байты в frontCanary
        for (int i = 0; i != 64; i++)
            frontCanary |= ( (unsigned long long)(rand() & 1) << i );
    }

    while (!backCanary)
    {
        //Запихаем рандомные байты в backCanary
        for (int i = 0; i != 64; i++)
            backCanary |= ( (unsigned long long)(rand() & 1) << i );
    }

    while (!poison)
    {
        //Запихаем рандомные байты в poison
        for (int i = 0; i != 64; i++)
            poison |= ( (unsigned long long)(rand() & 1) << i );
    }

#if ( defined(_STACK_DUPLICATE) && defined(_STACK_DUPLICATE_OFFSET) )
    duplicateOffset = rand() % (maxAbsOffset * 2) - maxAbsOffset;
#endif

}

#ifdef _STACK_HASH
static long long getHash(Stack* stack)
{
    //TODO Проверка адреса возврата
    assert(stack);
    static long long base = 0;

    //Установка base
    if (!base)
    {
        for (int i = 0; i != 64; i++)
            base |= ( (unsigned long long)(rand() & 1) << i );

        base |= 1ll;
    }

    unsigned long long* data = stack->data;

    unsigned long long hash = 0;
    size_t size = stack->size;
    for (int i = 0; i != size; i++)
    {
        hash *= base;
        hash += *(++data);
    }

    return hash;
}
#endif


StackStatus stackIsValid(Stack* stack)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    STATIC_VALUES();
    CHECK(stack == NULL);

    //Дефолтные проверки
    CHECK(stack->data == NULL);

    CHECK(!stack->capacity);

    CHECK(stack->size > stack->capacity);

    //Канарейки и яд
    CHECK(stack->data[0] != frontCanary);

    CHECK(stack->data[stack->size + 1] != backCanary);

    size_t const realCapacity = stack->capacity + 2;
    unsigned long long* data = stack->data + stack->size + 2;
    for (int i = stack->size + 2; i != realCapacity; i++)
    {
        CHECK(*(data++) != poison);
    }

#ifdef _STACK_HASH
    CHECK(getHash(stack) != stack->hash);
#endif

    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */



#ifdef _STACK_DUPLICATE
    Stack* duplicate = stack->duplicate;

    CHECK(duplicate == NULL);
    duplicate -= duplicateOffset;
    CHECK(
        duplicate->capacity != stack->capacity ||
        duplicate->size     != stack->size ||
        duplicate->hash     != stack->hash ||
        duplicate->data     == NULL ||
        duplicate->duplicate
    );

    unsigned long long* _data = duplicate->data;
    data = stack->data;

    for (int i = 0; i != realCapacity; i++)
        CHECK(*(data++) != *(_data++));
#endif


    return STACK_OK;
}


static Stack* allocateStack(size_t capacity)
{
    Stack* stack = (Stack*)calloc(1, sizeof(Stack));
    if (stack == NULL)
        return NULL;

    stack->capacity = capacity;
    stack->size = 0;
    stack->duplicate = NULL;
    stack->hash = 0;
    size_t realCapacity = capacity + 2;
    stack->data = calloc(realCapacity, sizeof(unsigned long long));
    if (stack->data == NULL)
    {
        free(stack);
        return NULL;
    }

    return stack;
}


static Stack* makeStackDuplicate(Stack* stack)
{
    if (stack == NULL || stack->data == NULL)
        return NULL;

    STATIC_VALUES();
    Stack* copy = allocateStack(stack->capacity);

    if (copy == NULL)
        return NULL;

    copy->hash = stack->hash;
    copy->size = stack->size;
    size_t realCapacity = stack->capacity + 2;

    unsigned long long* copyData  = copy-> data;
    unsigned long long* stackData = stack->data;

    for (int i = 0; i != realCapacity; i++)
        *(copyData++) = *(stackData++);

    return copy;
}


Stack* stackNew(size_t capacity)
{
    Stack* stack = allocateStack(capacity);
    if (stack == NULL)
        return NULL;

    STATIC_VALUES();
    size_t realCapacity = capacity + 2;

    unsigned long long* data = stack->data;
    for (int i = 0; i != realCapacity; i++)
        *(data++) = poison;
    stack->data[0] = frontCanary;
    stack->data[1] = backCanary;
#ifdef _STACK_DUPLICATE
    Stack* duplicate = makeStackDuplicate(stack);
    if (duplicate == NULL)
    {
        stackDelete(stack);
        return NULL;
    }
    stack->duplicate = duplicate + duplicateOffset;
#endif
    return stack;
}


void stackDelete(Stack* stack)
{
    if (stack == NULL)
        return;
    free(stack->data);
    if (stack->duplicate)
    {
        free((stack->duplicate - duplicateOffset)->data);
        free(stack->duplicate - duplicateOffset);
    }
    free(stack);
}


StackStatus stackResize(Stack* stack, size_t capacity)
{
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);

    if (capacity == NULL)
        return STACK_RESIZE_ERROR;

    if (capacity < stack->size)
        return STACK_OVERFLOW;

    unsigned long long* newData = (unsigned long long*)realloc(stack->data, (capacity + 2)*sizeof(unsigned long long));
    if (newData == NULL)
        return STACK_RESIZE_ERROR;
    size_t realCapacity = capacity + 2;

    for (size_t i = stack->capacity + 2; i < realCapacity; i++)
        newData[i] = poison;

    stack->data = newData;
    stack->capacity = capacity;
#ifdef _STACK_DUPLICATE
    DUPLICATE(stack, duplicate);
#endif

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackPush(Stack* stack, unsigned long long value)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);

    if (stack->size == stack->capacity)
        return STACK_OVERFLOW;

    stack->data[1 + stack->size++] = value;
    stack->data[1 + stack->size] = backCanary;
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */
    stack->hash = getHash(stack);

#ifdef _STACK_DUPLICATE
    DUPLICATE(stack, duplicate);
#endif

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackPop(Stack* stack)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);
    if (!stack->size)
        return STACK_UNDERFLOW;

    stack->data[stack->size] = backCanary;
    stack->data[stack->size + 1] = poison;
    stack->size--;
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */
    stack->hash = getHash(stack);

#ifdef _STACK_DUPLICATE
    DUPLICATE(stack, duplicate);
#endif

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackSize(Stack* stack, size_t* value)
{
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);
    *value = stack->size;

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackTop(Stack* stack, unsigned long long* value)
{
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);

    if (!stack->size)
        return STACK_UNDERFLOW;

    *value = stack->data[stack->size];

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}

StackStatus stackClear(Stack* stack)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);

    stack->data[1] = backCanary;
    unsigned long long* data = stack->data + 2;
    size_t capacity = stack->capacity;
    for (int i = 0; i != capacity; i++)
        *(data++) = poison;

    stack->size = 0;
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */
    stack->hash = getHash(stack);

#ifdef _STACK_DUPLICATE
    DUPLICATE(stack, duplicate);
#endif

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}

Stack* stackCopy(Stack* stack)
{
    if (stack == NULL || stack->data == NULL)
        return NULL;

    STATIC_VALUES();

    Stack* copy = makeStackDuplicate(stack);
    if (copy == NULL)
        return NULL;

#ifdef _STACK_DUPLICATE
    copy->duplicate = makeStackDuplicate(stack->duplicate - duplicateOffset);

    if (copy->duplicate == NULL)
    {
        stackDelete(copy);
        return NULL;
    }

    copy->duplicate += duplicateOffset;
#endif

    return copy;
}


StackStatus stackRescue(Stack* stack)
{
    if (stack == NULL)
        return STACK_NULL;
    STATIC_VALUES();

#ifdef _STACK_DUPLICATE
    if (stack->duplicate == NULL)
        return STACK_RESCUE_ERROR;
    Stack* backup = stack->duplicate - duplicateOffset;
    backup->duplicate = makeStackDuplicate(backup) + duplicateOffset;
    if (stackIsValid(backup) == STACK_INVALID)
    {
        stackDelete(backup->duplicate - duplicateOffset);
        backup->duplicate = NULL;
        return STACK_INVALID;
    }


    free(stack->data);
    *stack = *backup;
    return STACK_OK;
#else
    return STACK_RESCUE_ERROR;
#endif
}

