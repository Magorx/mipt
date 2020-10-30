#include <stdlib.h>

#define LIST_TYPE int
#include "list.h"
#undef LIST_TYPE

int main() {
	List *l = new_List();
	delete_List(l);

	printf("done.\n");
	return 0;
}