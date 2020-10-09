#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Stack.h"
#include "Stack.c"

void printStatus(StackStatus s)
{
	switch (s)
	{
	case STACK_OK:
		printf("STACK_OK\n");
		return;
	case STACK_NULL:
		printf("STACK_NULL\n");
		return;
	case STACK_OVERFLOW:
		printf("STACK_OVERFLOW\n");
		return;
	case STACK_UNDERFLOW:
		printf("STACK_UNDERFLOW\n");
		return;
	case STACK_INVALID:
		printf("STACK_INVALID\n");
		return;
	case STACK_DUPLICATION_ERROR:
		printf("STACK_DUPLICATION_ERROR\n");
		return;
	case STACK_RESIZE_ERROR:
		printf("STACK_RESIZE_ERROR\n");
		return;
	case STACK_RESCUE_ERROR:
		printf("STACK_RESCUE_ERROR\n");
		return;
	default:
		printf("DEFAULT\n");
	}
}

int main(void)
{
	int size = 0;
	scanf("%d", &size);
	assert(size > 0);
	Stack* st = stackNew(size);
	assert(st);
	char req[10] = "";
	long long value = 0;
	size_t nValue = 0;

	void (*foo)(void) = NULL;

	while(1)
	{
		scanf("%9s", req);
        st->capacity--;
        (st->duplicate - duplicateOffset)->capacity--;
		if (!strcmp("resize", req))
		{
			scanf("%d", &nValue);
			printStatus(stackResize(st, nValue));
			continue;
		}

		if (!strcmp("isvalid", req))
		{
			printStatus(stackIsValid(st));
			continue;
		}

		if (!strcmp("push", req))
		{
			scanf("%lld", &value);
			printStatus(stackPush(st, value));
			continue;
		}

		if (!strcmp("pop", req))
		{

			printStatus(stackPop(st));
			continue;
		}

		if (!strcmp("size", req))
		{

			printStatus(stackSize(st, &value));
			printf("\t%lld\n", value);
			continue;
		}

		if (!strcmp("top", req))
		{

			printStatus(stackTop(st, &value));
			printf("\t%lld\n", value);
			continue;
		}

		if (!strcmp("clear", req))
		{

			printStatus(stackClear(st));
			continue;
		}

		if (!strcmp("hack", req))
		{
			int i = 0;
			printf("i\tvalue\n");
			scanf("%d%lld", &i, &value);
			st->data[i] = value;
			continue;
		}

		if (!strcmp("call", req))
		{
			scanf("%p", foo);
			foo();
			continue;
		}

		if (!strcmp("delete", req))
		{
			stackDelete(st);
			return 0;
		}

		if (!strcmp("rescue", req))
		{
			printStatus(stackRescue(st));
			continue;
		}

		if (!strcmp("copy", req))
		{
			Stack* copy = stackCopy(st);
			stackDelete(st);
			st = copy;
			continue;
		}

		if (!strcmp("duplbrk", req))
		{
			st->duplicate = NULL;
			continue;
		}

		printf("unknown req\n");
	}
}
