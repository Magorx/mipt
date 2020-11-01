#include <stdlib.h>

#define LIST_TYPE int
#include "list.h"
#undef LIST_TYPE

#define TIME_MEASURED(code){clock_t begin = clock();code;clock_t end = clock();\
double elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;\
printf("ELAPSED: %lf sec\n", elapsed_secs);}

int main() {
	List *l = new_List();
	
	printf("Pushing\n");
	TIME_MEASURED({

	List_push_right(l, 1, -1);
	for (int i = 1; i <= 100000000; ++i) {
		if (rand() % 2) {
			List_push_front(l, i);
		} else {
			List_push_back(l, i);
		}
		if (rand() % 2) {
			//List_pop(l, l->head);
		}
	}

	});

	printf("Sorting\n");
	TIME_MEASURED({
	List_sort(l);
	});

	//List_dump(l);

	delete_List(l);

	printf("done.\n");
	return 0;
}