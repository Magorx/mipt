#include <cstdio>
#include <cstdlib>

#include "hashtable.h"
#include "general/c/random.h"

const int TEST_CNT = 5;
const int TEST_SIZE = 100000;

const size_t TEST_INIT_BUCKET_SIZE = 3;
const size_t TEST_INIT_CAPACITY = 64;

char *read_file(const char *filename) {
	FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("[ERR]<read_file>: [%s] is not found\n", filename);
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
        //     printf("%s %lu %lu\n", key, MAX_KEY_LEN, strlen(key));
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

int test_hashtable_find() {
	Hashtable ht;
    ht.ctor(TEST_INIT_BUCKET_SIZE, TEST_INIT_CAPACITY);

    printf("Reading dict\n");
    TIMER_START();
    char *data = read_file("../dict_en_rus_cp1251.dic");
    // printf("%s\n", data);
    int dict_size = 0;
    HT_Node *nodes = cut_dict_to_nodes(data, &dict_size);

    ht.execute_queue(nodes, HT_Q_INSERT_ONLY, dict_size, HT_Q_IGNORANCE_BUFFER);
    TIMER_END_AND_PRINT();

    printf("Dict read\n");

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

    printf("[TST]<find>: %g\n", GLOBAL_TIMER_INTERVAL / TEST_CNT);
    ht.dtor();
    free(nodes);
    free(data);
    free(find_results);

    return 0;
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
