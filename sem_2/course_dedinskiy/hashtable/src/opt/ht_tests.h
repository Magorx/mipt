#include <cstdio>
#include <cstdlib>

#include "hashtable.h"
#include "../general/c/random.h"

const int TEST_CNT = 5;
const int TEST_SIZE = 1000000;

const int INSERT_TEST_SIZE_DIVISOR = 10;

const size_t TEST_INIT_BUCKET_SIZE = 3;
const size_t TEST_INIT_CAPACITY = 64;

char *read_file(const char *filename) {
	FILE *file = fopen(filename, "rb");
    if (!file) {
        #ifndef NO_PRINTS
            printf("[ERR]<read_file>: [%s] is not found\n", filename);
        #endif
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    auto data = (char*) calloc(fsize + 2, 1);
    if (!data) {
        return nullptr;
    }

    size_t go_away = fread(data, sizeof(char), fsize, file);
    if (go_away == (size_t)-1) {
        free(data);
        return nullptr;
    }
    fclose(file);
    data[fsize] = 0;
    return data;
}

HT_Node *cut_dict_to_nodes(char *data, int *dict_size) {
    assert(data);
    assert(dict_size);

	char *dt = data;

    int pairs_cnt = 0;
    while (*dt) {
        char c = *dt;
        if (c == '=' || c == '\n' || c == '\r') {
            pairs_cnt += c == '=';
            *dt = '\0';
        }
        ++dt;
    }

    HT_Node *nodes = (HT_Node*) calloc(pairs_cnt, sizeof(HT_Node));
    assert(nodes);

    for (int i = 0; i < pairs_cnt; ++i) {
        char *key = data;
        char *val = data + (strlen(data) + 1);
        nodes[i].copy_from({key, val, 0});  
        data = val + (strlen(val) + 2);
        // if (strlen(key) > MAX_KEY_LEN) {
        //     #ifndef NO_PRINTS
        //     printf("%s %lu %lu\n", key, MAX_KEY_LEN, strlen(key));
        // #endif
        // }
    }

    *dict_size = pairs_cnt;

    return nodes;
}

HT_Node *prepare_random_find_test(HT_Node *nodes, const size_t node_cnt, const size_t test_size) {
    assert(nodes);

    HT_Node *test = (HT_Node*) calloc(test_size, sizeof(HT_Node));
    assert(test);

    for (size_t i = 0; i < test_size; ++i) {
        test[i].copy_from(nodes[randlong() % node_cnt]);
    }

    return test;
}

float test_hashtable_find() {
    #ifndef NO_PRINTS
        printf("Running <find> test -----\n");
    #endif

	Hashtable ht;
    ht.ctor(TEST_INIT_BUCKET_SIZE, TEST_INIT_CAPACITY);

    #ifndef NO_PRINTS
        printf("Reading dict\n");
        TIMER_START();
    #endif

    char *data = read_file("../dict_en_rus_cp1251.dic");
    int dict_size = 0;
    HT_Node *nodes = cut_dict_to_nodes(data, &dict_size);

    ht.execute_queue(nodes, HT_Q_INSERT_ONLY, dict_size, HT_Q_IGNORANCE_BUFFER);

    #ifndef NO_PRINTS
        TIMER_END_AND_PRINT();
        printf("Dict read\n");
    #endif

    HT_Node *tests[TEST_CNT] = {};
    for (int i = 0; i < TEST_CNT; ++i) {
        tests[i] = prepare_random_find_test(nodes, dict_size, TEST_SIZE);
    }

    const char **find_results = (const char**) calloc(TEST_SIZE, sizeof(char*));

    TIMER_START();
    for (int i = 0; i < TEST_CNT; ++i) {
        HT_Node *test = tests[i];
        ht.execute_queue(test, HT_Q_FIND_ONLY, TEST_SIZE, find_results);
        free(test);
    }
    TIMER_BREAK();

    #ifndef NO_PRINTS
        printf("[TST]<find>: %g\n", GLOBAL_TIMER_INTERVAL / TEST_CNT);
    #endif
    ht.dtor();
    free(nodes);
    free(data);
    free(find_results);

    return GLOBAL_TIMER_INTERVAL / TEST_CNT;
}

HT_Node *prepare_random_insert_test(HT_Node *nodes, const size_t node_cnt, const size_t test_size) {
    assert(nodes);

    HT_Node *test = (HT_Node*) calloc(test_size, sizeof(HT_Node));
    assert(test);

    for (size_t i = 0; i < test_size; ++i) {
        test[i].copy_from(nodes[randlong() % node_cnt]);
        test[i].key[rand() % MAX_KEY_LEN] += 1;
    }

    return test;
}

float test_hashtable_insert() {
    #ifndef NO_PRINTS
        printf("Running <insert> test -----\n");
    #endif

    Hashtable ht;
    ht.ctor(TEST_INIT_BUCKET_SIZE, TEST_INIT_CAPACITY);

    #ifndef NO_PRINTS
        printf("Reading dict\n");
        TIMER_START();
    #endif
    char *data = read_file("../dict_en_rus_cp1251.dic");
    int dict_size = 0;
    HT_Node *dict = cut_dict_to_nodes(data, &dict_size);

    ht.execute_queue(dict, HT_Q_INSERT_ONLY, dict_size, HT_Q_IGNORANCE_BUFFER);

    #ifndef NO_PRINTS
        TIMER_END_AND_PRINT();
        printf("Dict read\n");
    #endif

    HT_Node *tests[TEST_CNT] = {};
    tests[0] = prepare_random_insert_test(dict, dict_size, TEST_SIZE / INSERT_TEST_SIZE_DIVISOR);
    for (int i = 1; i < TEST_CNT; ++i) {
        tests[i] = prepare_random_insert_test(tests[i - 1], TEST_SIZE / INSERT_TEST_SIZE_DIVISOR, TEST_SIZE / INSERT_TEST_SIZE_DIVISOR);
    }

    TIMER_START();
    for (int i = 0; i < TEST_CNT; ++i) {
        HT_Node *test = tests[i];
        ht.execute_queue(test, HT_Q_INSERT_ONLY, TEST_SIZE / INSERT_TEST_SIZE_DIVISOR, HT_Q_IGNORANCE_BUFFER);
    }
    TIMER_BREAK();

    #ifndef NO_PRINTS
        printf("[TST]<insert>: %g\n", GLOBAL_TIMER_INTERVAL / TEST_CNT);
        printf("[   ]<      >: new_size: %lu\n", ht.get_size());
    #endif

    return GLOBAL_TIMER_INTERVAL / TEST_CNT;
}
