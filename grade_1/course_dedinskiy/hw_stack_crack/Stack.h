/*
Copyright (c) 2020  MIPT
Module Name:
    Stack.h
Abstract:
    Описывает класс стека для 64-битных переменных
Author:
    JulesIMF
Last Edit:
    07.10.2020 16:39
Edit Notes:

*/


#pragma once
#ifndef JULESIMF_STACK

#define JULESIMF_STACK
#include <stdlib.h>

typedef enum
{
    STACK_OK,
    STACK_NULL,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    STACK_INVALID,
    STACK_DUPLICATION_ERROR,
    STACK_RESIZE_ERROR,
    STACK_RESCUE_ERROR
} StackStatus;


typedef struct Stack__
{
    unsigned long long* data;
    size_t capacity;
    size_t size;
    unsigned long long hash;
    struct Stack__* duplicate;
} Stack;

Stack*          stackNew        (size_t capacity);
void            stackDelete     (Stack* stack);
StackStatus     stackIsValid    (Stack* stack);
StackStatus     stackPush       (Stack* stack, unsigned long long value);
StackStatus     stackPop        (Stack* stack);
StackStatus     stackSize       (Stack* stack, size_t* value);
StackStatus     stackTop        (Stack* stack, unsigned long long* value);
StackStatus     stackClear      (Stack* stack);
StackStatus     stackResize     (Stack* stack, size_t capacity);
Stack*          stackCopy       (Stack* stack);
StackStatus     stackRescue     (Stack* stack);


#endif // !JULESIMF_STACK

