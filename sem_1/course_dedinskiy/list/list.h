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

	int max_sorted_index;
} List;

enum LIST_ERROR_CODES {
	ERROR_BROCKEN_LINKS = -7777,
	ERROR_UNEXPECTED_LOOP,
	ERROR_NODE_NOT_IN_LIST,
};

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

	{
		char *visited = calloc(cake->capacity, sizeof(char));
		for (int i = cake->head; (int) i != cake->tail; i = cake->buffer[i].next) {
			if (visited[i]) {
				RETURNING_VERIFY(ERROR_UNEXPECTED_LOOP);
			}
			visited[i] = 1;
		}
		visited[cake->tail] = 1;
		int visited_sum = 0;
		for (size_t i = 0; i < cake->capacity; ++i) {
			visited_sum += (int) visited[i];
		}
		if (visited_sum != (int) cake->size && cake->size != 0) {
			RETURNING_VERIFY(ERROR_BROCKEN_LINKS);
		}
		free(visited);
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

;	free(cake->buffer);
	
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

	if (cake->size >= capacity - 1) {
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
	cake->buffer[node].next = node;
	cake->buffer[node].prev = node;

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
		RETURNING_VERIFY(ERROR_NODE_NOT_IN_LIST);
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
		RETURNING_VERIFY(ERROR_NODE_NOT_IN_LIST);
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

int List_push_front(List *cake, LIST_TYPE data) {
	return List_push_left(cake, cake->head, data);
}

int List_push_back(List *cake, LIST_TYPE data) {
	return List_push_right(cake, cake->tail, data);
}

int List_pop(List *cake, const int node) {
	VERIFY_OK(List_valid(cake));
	VERIFY(cake->size > 0);

	int next_node = cake->buffer[node].next;
	int prev_node = cake->buffer[node].prev;

	if (node == cake->head) {
		cake->head = next_node;
	}
	if (node == cake->tail) {
		cake->tail = prev_node;
	}

	cake->buffer[node].next = prev_node;
	cake->buffer[node].prev = next_node;

	//cake->buffer[node].data = (LIST_TYPE) KCTF_POISON;
	--cake->size;

	return 0;
}

int List_sort(List *cake) {
	VERIFY_OK(List_valid(cake));

	Node *new_buffer = calloc(cake->capacity, sizeof(Node));
	int node_index = cake->head;
	int inted_size = (int) cake->size;
	for (int i = 0; i < inted_size; ++i) {
		//printf("%d\n", i);
		//memcpy(&new_buffer[i + 1], &cake->buffer[node_index], sizeof(Node));
		new_buffer[i + 1] = cake->buffer[node_index];
		new_buffer[i + 1].next = i + 2;
		new_buffer[i + 1].prev = i;
		node_index = cake->buffer[node_index].next;
	}
	free(cake->buffer);

	cake->head = 1;
	cake->tail = inted_size;
	cake->buffer = new_buffer;

	cake->max_sorted_index = inted_size;

	return 0;
}