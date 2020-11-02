#include <stdlib.h>

#define LIST_TYPE int
#include "list.h"
#undef LIST_TYPE

#define TIME_MEASURED(code){clock_t begin = clock();{code};clock_t end = clock();\
double elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;\
printf("ELAPSED: %lf sec\n", elapsed_secs);}

int main() {
	srand((unsigned int) time(NULL));
	List *l = new_List();
	int N = 10;
	
	printf("Pushing\n");

	TIME_MEASURED(

	for (int i = 1; i <= N; ++i) {
		if (rand() % 2) {
			List_push_front(l, i);
		} else {
			List_push_back(l, i);
		}
		if (rand() % 2) {
			//List_pop(l, l->head);
		}
	}

	);

	//List_dump(l);

	printf("Sorting\n");
	TIME_MEASURED(

	//List_linear_optimization(l);

	);

	List_graphviz_dump(l, "gv_dump_");
	List_dump(l);

	delete_List(l);

	printf("done.\n");
	return 0;
}