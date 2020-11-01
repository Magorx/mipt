#include <stdlib.h>

#include "general.h"

#ifndef LIST_TYPE
#error LIST_TYPE must be defined to work with KCTF_List
#endif

struct Node_t;
typedef struct Node_t {
	int prev;
	LIST_TYPE data;
	int next;
} Node;

typedef struct List_t {
	Node *buffer;

	size_t capacity;
	size_t size;
	int head;
	int tail;
	int free_head;
} List;

/*enum LIST_RETURN_CODES {
	LIST_
};*/

List *new_List();
int   delete_List();

int List_push_next(List *cake, Node *node, LIST_TYPE data);
int List_push_prev();

int List_push_front();
int List_push_back();

//=============================================================================
// Implementation =============================================================

int List_valid(const List *cake) {
	if (!cake) {
		RETURNING_VERIFY(ERROR_NULL_OBJECT);
	}

	if (!cake->buffer) {
		RETURNING_VERIFY(ERROR_NULL_BUFFER);
	}

	return 0;
}

List *new_List() {
	size_t cap = STANDART_INIT_SIZE;
	
	List *l = calloc(1, sizeof(List));
	if (!l) {
		return NULL;
	}

	l->buffer = calloc(cap, sizeof(Node));
	if (!l->buffer) {
		free(l);
		return NULL;
	}

	l->capacity = cap;
	l->size = 0;
	l->head = 1;
	l->tail = 1;
	l->free_head = l->head;

	for (size_t i = 1; i < cap; ++i) {
		l->buffer[i].next = (int) i + 1;
		// printf("%d\n", i);
		l->buffer[i].prev = 0;
	}

	VERIFY_T(List_valid(l) == OK, List*);
	return l;
}

int delete_List(List *cake) {
	VERIFY_OK(List_valid(cake));

	free(cake->buffer);
	
	cake->capacity  = (size_t) KCTF_POISON;
	cake->head      = (int) KCTF_POISON;
	cake->tail      = (int) KCTF_POISON;

	free(cake);

	return 0;
}

int List_dump(const List *cake) {
	VERIFY_OK(List_valid(cake));

	for (int i = cake->head; i != cake->tail; i = cake->buffer[i].next) {
		printf("%d -> ", cake->buffer[i].data);
	}
	printf("%d -> ", cake->buffer[cake->tail].data);
	printf("\n");

	return 0;
}

int List_set_capacity(List *cake, const size_t capacity) {
	VERIFY_OK(List_valid(cake));

	if (cake->size <= capacity - 1) {
		RETURNING_VERIFY(ERROR_REALLOC_FAIL);
	}

	Node *new_ptr = realloc(cake->buffer, capacity * sizeof(Node));
	if (!new_ptr) {
		RETURNING_VERIFY(ERROR_REALLOC_FAIL);
	}

	// printf("%d -> %d\n", cake->buffer, new_ptr);

	cake->buffer = new_ptr;
	// printf("%d -> %d\n", cake->buffer, new_ptr);
	for (size_t i = cake->capacity; i < capacity; ++i) {
		// printf("%d\n", i);
		cake->buffer[i].next = (int) i + 1;
		cake->buffer[i].prev = 0;
	}
	cake->capacity = capacity;

	return OK;
}

int List_set_node_value(List *cake, const int node, const int left_node, const int right_node, const LIST_TYPE *data) {
	cake->buffer[node].data = *data;
	cake->buffer[node].next = 0;
	cake->buffer[node].prev = 0;

	if (left_node) {
		cake->buffer[left_node].next = node;
		cake->buffer[node].prev  = left_node;
	} else {
		cake->head = node;
	}

	if (right_node) {
		cake->buffer[right_node].prev = node;
		cake->buffer[node].next  = right_node;
	} else {
		cake->tail = node;
	}

	return 0;
}

int List_push_right(List *cake, int node, LIST_TYPE data) {
	VERIFY_OK(List_valid(cake));
	if (cake->buffer[node].prev == (int) KCTF_POISON) {
		RETURNING_VERIFY(ERROR_BAD_NODE_INDEX);
	}

	int next_free = cake->free_head;
	if ((size_t) next_free >= cake->capacity - 1) {
		VERIFY_OK(List_set_capacity(cake, cake->capacity * 2));
	}

	cake->free_head = cake->buffer[next_free].next;

	int next_node = cake->buffer[node].next * (node != cake->tail);
	List_set_node_value(cake, next_free, node, next_node, &data);
	cake->size += 1;

	return 0;
}

int List_push_left(List *cake, int node, LIST_TYPE data) {
	VERIFY_OK(List_valid(cake));
	if (cake->buffer[node].prev == (int) KCTF_POISON) {
		RETURNING_VERIFY(ERROR_BAD_NODE_INDEX);
	}

	int next_free = cake->free_head;
	if ((size_t) next_free >= cake->capacity - 1) {
		VERIFY_OK(List_set_capacity(cake, cake->capacity * 2));
	}

	cake->free_head = cake->buffer[next_free].next;

	int prev_node = cake->buffer[node].prev * (node != cake->head);
	List_set_node_value(cake, next_free, prev_node, node, &data);
	cake->size += 1;

	return 0;
}
