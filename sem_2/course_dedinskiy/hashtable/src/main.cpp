#include <cstdio>
#include <cstdlib>

#define NO_PRINTS

#include "general/c/timer.h"

#include "opt/hashtable.h"
#include "opt/ht_tests.h"

const int RAND_CONST_SEED = 123;

int main() {
	srand(RAND_CONST_SEED);
	
	float find_time   = test_hashtable_find  ();
	float insert_time = test_hashtable_insert();

	printf("[TST]<find  >: %g\n", find_time  );
	printf("[TST]<insert>: %g\n", insert_time);

	return 0;
}
