#include <stdlib.h>

#include "general.h"

#ifndef LIST_TYPE
#error LIST_TYPE must be defined to work with KCTF_List
#endif

typedef struct Node_t {
	size_t prev;
	LIST_TYPE data;
	size_t next;
} Node;

typedef struct List_t {
	Node *buffer;
	size_t capacity;
	size_t head;
	size_t tail;
} List;

/*enum LIST_RETURN_CODES {
	LIST_
};*/

List *new_List();
int   delete_List();

int List_push_next();
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
	l->head = 1;
	l->tail = 1;

	VERIFY_T(List_valid(l) == OK, List*);
	return l;
}

int delete_List(List *cake) {
	VERIFY_OK(List_valid(cake));

	free(cake->buffer);
	
	cake->capacity  = (size_t) KCTF_POISON;
	cake->head      = (size_t) KCTF_POISON;
	cake->tail      = (size_t) KCTF_POISON;

	free(cake);

	return 0;
}

