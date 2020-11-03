#include "list_tests.h"

int main() {
	printf("[TST]<unit_test>: starting\n");

	KCTF_UNIT_TEST_RUN(head_tail);
	KCTF_UNIT_TEST_RUN(set_capacity);
	
	printf("[TST]<unit_test>: done\n");

	return 0;
}