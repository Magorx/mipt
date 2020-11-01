#include <stdlib.h>

#define LIST_TYPE int
#include "list.h"
#undef LIST_TYPE

int main() {
	List *l = new_List();
	List_push_right(l, 1, -1);

	for (int i = 1; i <= 1000; ++i) {
		if (rand() % 2) {
			List_push_right(l, rand() % l->size + 1, i);
		} else {
			List_push_left(l, rand() % l->size + 1, i);
		}
	}

	for (int i = 0; i < 5; ++i) {
		List_push_left(l, rand() % 10 + 1, i * 10);
	}

	List_dump(l);

	delete_List(l);

	printf("done.\n");
	return 0;
}