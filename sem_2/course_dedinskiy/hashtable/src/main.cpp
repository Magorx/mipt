#include <cstdio>
#include <cstdlib>
#include <clocale>

#include "general/c/timer.h"

#include "hashtable.h"
#include "ht_tests.h"

int main() {
	srand(123);
	
	test_hashtable_find();
	test_hashtable_insert();

	return 0;
}
