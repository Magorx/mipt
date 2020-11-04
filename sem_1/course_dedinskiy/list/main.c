#include <stdlib.h>

#define LIST_TYPE int
#include "list.h"
#undef LIST_TYPE

#define X 1

#define TIME_MEASURED(code){clock_t begin = clock();{code};clock_t end = clock();\
double elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;\
printf("ELAPSED: %lf sec\n", elapsed_secs);}

int main() {
	srand((unsigned int) time(NULL));
	List *l = new_List();
	int N = 10;
	
	printf("Pushing\n");

	TIMER_START();
	for (int i = 1; i <= N; ++i) {
		int roll = rand() % 3;
		if (roll == 0) {
			List_push_front(l, i);
		} else if (roll == 1){
			List_push_back(l, i);
		} else if (roll == 2 && l->size) {
			List_pop(l, List_linear_index_search(l, rand() % ((int) l->size)));
		}
		List_graphviz_dump(l, "gv_dump_");
	}
	TIMER_END_AND_PRINT();

	printf("Optimizing\n");
	List_linear_optimization(l);
	TIMER_END_AND_PRINT();

	List_graphviz_generate_html(l, "gv_dump_");
	//List_dump(l);

	delete_List(l);



	return 0;
}