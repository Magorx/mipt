#define LIST_TYPE int
#include "list.h"
#undef LIST_TYPE

const int TEST_LOOP_MAX_ITR = 1000;

KCTF_UNIT_TEST(head_tail) {
	List *l = new_List();

	EXPECT_EQ(l->buffer[l->fictive].next, List_head(l));
	EXPECT_EQ(l->buffer[l->fictive].prev, List_tail(l));

	for (int i = 0; i < TEST_LOOP_MAX_ITR; ++i) {
		List_randop(l);
		EXPECT_EQ(l->buffer[l->fictive].next, List_head(l));
		EXPECT_EQ(l->buffer[l->fictive].prev, List_tail(l));
	}

	delete_List(l);
}

KCTF_UNIT_TEST(set_capacity) {
	List *l = new_List();

	for (int i = 64; i < TEST_LOOP_MAX_ITR; i = i + rand() % 32) {
		List_set_capacity(l, (size_t) i);
		EXPECT_EQ(l->capacity, (size_t) i);
	}

	delete_List(l);
}

KCTF_UNIT_TEST(fail) {
	EXPECT_TRUE(0);
}