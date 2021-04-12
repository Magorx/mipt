#include <cstdio>
#include <cstdlib>
#include <clocale>

#include "general/c/timer.h"

#include "hashtable.h"
#include "ht_tests.h"

int main() {
	srand(123);
	// test_hashtable_find();

	Hashtable ht;
    ht.ctor(TEST_INIT_BUCKET_SIZE, TEST_INIT_CAPACITY);

    printf("Reading dict\n");
    TIMER_START();
    char *data = read_file("../dict_en_rus_cp1251.dic");
    int dict_size = 0;
    HT_Node *dict = cut_dict_to_nodes(data, &dict_size);

    ht.execute_queue(dict, HT_Q_INSERT_ONLY, dict_size, HT_Q_IGNORANCE_BUFFER);
    TIMER_END_AND_PRINT();

    printf("Dict read\n");

    HT_Node *tests[TEST_CNT] = {};
    tests[0] = prepare_random_insert_test(dict, dict_size, TEST_SIZE);
    for (int i = 1; i < TEST_CNT; ++i) {
        tests[i] = prepare_random_insert_test(tests[i - 1], TEST_SIZE, TEST_SIZE);
    }

    TIMER_START();
    for (int i = 0; i < TEST_CNT; ++i) {
        HT_Node *test = tests[i];
        ht.execute_queue(test, HT_Q_INSERT_ONLY, TEST_SIZE, HT_Q_IGNORANCE_BUFFER);
        printf("cur_size: %lu\n", ht.get_size());
    }
    TIMER_BREAK();

    printf("[TST]<insert>: %g\n", GLOBAL_TIMER_INTERVAL / TEST_CNT);

    printf("cur_size: %lu\n", ht.get_size());

	return 0;
}
