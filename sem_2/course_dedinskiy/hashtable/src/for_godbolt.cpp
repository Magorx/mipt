#ifndef HASHTABLE
#define HASHTABLE

#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>


typedef int RETURN_CODE;
RETURN_CODE OK = 0;
RETURN_CODE ERROR_BAD_OBJECT = 1;
RETURN_CODE ERROR_MALLOC_FAIL = 2;
RETURN_CODE ERROR_REALLOC_FAIL = 3;
#define VERIFY_T(a, b)
#define VERIFY_OK(a)
typedef const char* HT_RET_TYPE;

enum NODE_ALIVENESS {
	IS_DEAD = 0,
	IS_DELETED = 2,
	IS_ALIVE = 3,
};

const uint64_t MAX_KEY_LEN = 32;

struct HT_Node {
	char key[MAX_KEY_LEN];
	const char *val;

	uint64_t hashed;
	char     is_alive;

	HT_Node(const char *string, const char *value, const int) {
		assert(string != nullptr);

		// printf("NODE %s | %s\n", string, value);

		memset (key, 0, MAX_KEY_LEN);
		strncpy(key, string, MAX_KEY_LEN - 1);
		val = value;
		is_alive = IS_ALIVE;

		hashed = 0;

        
	}

	HT_Node(const char *string, const char *value) {
		assert(string != nullptr);

		memset (key, 0, MAX_KEY_LEN);
		strncpy(key, string, MAX_KEY_LEN - 1);
		val = value;
		is_alive = IS_ALIVE;

		hashed = 0;
		hash();
	}

	HT_Node(const char *string, const int) {
		assert(string != nullptr);

		memset (key, 0, MAX_KEY_LEN);
		strncpy(key, string, MAX_KEY_LEN - 1);
		val = nullptr;
		is_alive = IS_ALIVE;

		hashed = 0;
	}

	HT_Node(const char *string) {
		assert(string != nullptr);

		memset (key, 0, MAX_KEY_LEN);
		strncpy(key, string, MAX_KEY_LEN - 1);
		val = nullptr;
		is_alive = IS_ALIVE;

		hashed = 0;
		hash();
	}

	void ctor() {
		memset(key, 0, MAX_KEY_LEN);
		val = nullptr;
		is_alive = IS_DEAD;
	}

	void dtor() {
		memset(key, 0, MAX_KEY_LEN);
		val = nullptr;
		is_alive = IS_DEAD;
	}

	void copy_from(const HT_Node &node) {
		memcpy(key, node.key, MAX_KEY_LEN);
		val = node.val;
		is_alive = node.is_alive;
		hashed = node.hashed;
	}

	inline void count_copy(const HT_Node &node) {
		val = node.val;
	}

	uint64_t hash() {
		if (hashed) return hashed;

		hashed = 14695981039346656037llu;
		uint64_t prime = 1099511628211llu;
		for (size_t i = 0; i < MAX_KEY_LEN; ++i) {
			hashed = hashed ^ key[i];
			hashed = hashed * prime;
		}

		return hashed;
	}

	inline char is() const {
		return is_alive;
	}

	inline char set_is(const char new_alive) {
		return is_alive = new_alive;
	}

	inline bool eq(const HT_Node &second) {
		return strncmp(key, second.key, MAX_KEY_LEN) == 0;
	}
};

// inline int operator==(const HT_Node &first, const HT_Node &second) {
//     long ret = 0;
//     asm ("vmovdqu xmm0, [%1]\n"
//          "vmovdqu xmm1, [%2]\n"
//          "vpcmpeqw xmm0, xmm0, xmm1\n"
//          "VPTEST  xmm0, xmm0\n"
//          "xor rdx, rdx\n"
//          "setne dl\n"
//          "mov %0, rdx\n"
//     :"=r"(ret):"r"(first.key),"r"(second.key):"dl", "xmm0", "xmm1");
// 	//return memcmp(first.key, second.key, MAX_KEY_LEN) == 0;
//     return ret;
// }

inline bool operator==(const HT_Node &first, const HT_Node &second) {
	return memcmp(first.key, second.key, MAX_KEY_LEN) == 0;
}

const size_t REHASH_CAPACITY_INCREASE_RATE = 2;
const size_t REHASH_BUCKET_INCREASE_CNT = 1;
const size_t MIN_ADDITIONAL_CAPACITY = 5;

enum QUEUE_OPERATION {
	HT_Q_INSERT = 1,
	HT_Q_ERASE,
	HT_Q_FIND,
};

#define HT_Q_INSERT_ONLY (char*) HT_Q_INSERT
#define HT_Q_ERASE_ONLY (char*) HT_Q_ERASE
#define HT_Q_FIND_ONLY (char*) HT_Q_FIND

#define HT_Q_IGNORANCE_BUFFER (HT_RET_TYPE*) 1

class Hashtable {
private:
// data =======================================================================
	size_t size;
	size_t capacity;
	size_t bucket_size;
	size_t overall_data_size;
	HT_Node *data;

	float loadrate;
	float max_loadrate;
//=============================================================================

	RETURN_CODE rehash();
	RETURN_CODE check_loadrate();

public:
	 Hashtable();
	~Hashtable();

	RETURN_CODE ctor(const size_t capacity_ = 1000000, const size_t bucket_size_ = 10, const float max_loadrate_ = 0.5);
	RETURN_CODE dtor();
	
	static Hashtable *NEW(const size_t size_ = 1000000, const size_t bucket_size_ = 10, const float max_loadrate_ = 0.5) {
		Hashtable *cake = (Hashtable*) calloc(1, sizeof(Hashtable));
		if (!cake) {
			return nullptr;
		}

		VERIFY_T(cake->ctor(size_, bucket_size_, max_loadrate_) == OK, Hashtable*);
		return cake;
	}

	static RETURN_CODE DELETE(Hashtable *hashtable) {
		if (!hashtable) {
			return ERROR_BAD_OBJECT;
		}

		VERIFY_OK(hashtable->dtor());
		free(hashtable);

		return OK;
	}

	bool is_valid() const;

	RETURN_CODE insert (const HT_Node &node, bool to_check_loadrate = true);
	RETURN_CODE erase  (const HT_Node &node);
	HT_RET_TYPE	find   (const HT_Node &node) const;

	RETURN_CODE execute_queue(HT_Node *nodes, char *operations, size_t queue_size, HT_RET_TYPE *result);

	inline size_t get_size() const {
		return size;
	}
};

#endif // HASHTABLE


Hashtable::Hashtable()
{}

Hashtable::~Hashtable()
{}

RETURN_CODE Hashtable::ctor(const size_t capacity_, const size_t bucket_size_, const float max_loadrate_) {
	size = 0;
	capacity = capacity_;
	bucket_size = bucket_size_;
	loadrate = 0;
	max_loadrate = max_loadrate_;

	overall_data_size = capacity * bucket_size;

	data = (HT_Node*) calloc(overall_data_size, sizeof(HT_Node));
	if (!data) {
		return ERROR_MALLOC_FAIL;
	}

	for (size_t i = 0; i < overall_data_size; ++i) {
		data[i].ctor();
	}

	check_loadrate();

	return OK;
}

RETURN_CODE Hashtable::dtor() {
	if (data) {
		free(data);
	}
	data = nullptr;

	capacity = 0;
	bucket_size = 0;
	overall_data_size = 0;
	max_loadrate = 0;
	loadrate = 0;

	return OK;
}

bool Hashtable::is_valid() const {
	return !((capacity * bucket_size == overall_data_size) && (!overall_data_size || data));
}

RETURN_CODE Hashtable::rehash() {
	VERIFY_OK(is_valid());

	HT_Node *new_data = (HT_Node*) calloc((bucket_size + REHASH_BUCKET_INCREASE_CNT) * (capacity * REHASH_CAPACITY_INCREASE_RATE), sizeof(HT_Node));
	if (!new_data) {
		return ERROR_REALLOC_FAIL;
	}

	HT_Node *old_data = data;
	size_t old_ov_size = overall_data_size;
	
	data = new_data;
	bucket_size += REHASH_BUCKET_INCREASE_CNT;
	capacity *= REHASH_CAPACITY_INCREASE_RATE;
	overall_data_size = capacity * bucket_size;
	size = 0;

	for (size_t i = 0; i < old_ov_size; ++i) {
		const HT_Node &n = old_data[i];
		if (n.is() == IS_ALIVE) {
			insert(n, false);
		}
	}

	free(old_data);
	return OK;
}

RETURN_CODE Hashtable::check_loadrate() {
	loadrate = ((float) size) / overall_data_size;
	// printf("loadrate: %g\n", loadrate);
	if (loadrate > max_loadrate || overall_data_size - size < MIN_ADDITIONAL_CAPACITY) {
		return rehash();
	}

	return OK;
}

RETURN_CODE Hashtable::insert(const HT_Node &node, bool to_check_loadrate) {
	VERIFY_OK(is_valid());

	size_t hash = node.hashed % capacity;
	size_t coord = hash * bucket_size;

	// printf("hash_pos(%lu) for %s is %lu\n", node.hashed, node.key, hash);

	size_t max_i = bucket_size;
	size_t first_free_index = (size_t) -1;
	for (size_t i = 0; i < max_i; ++i, ++coord) {
		HT_Node &n = data[coord];
		if (n.is() < IS_ALIVE && (first_free_index == (size_t) -1)) {
			first_free_index = coord;
		}

		if (n.is() == IS_DEAD) {
			break;
		}

		if (n == node) {
			n.count_copy(node);
			return OK;
		}
	}

	if (first_free_index != (size_t) -1) {
		data[first_free_index].copy_from(node);
		++size;
		if (to_check_loadrate) VERIFY_OK(check_loadrate());
	} else {
		VERIFY_OK(rehash());
		VERIFY_OK(insert(node, true));
	}

	return OK;
}

RETURN_CODE Hashtable::erase(const HT_Node &node) {
	VERIFY_OK(is_valid());

	size_t hash = node.hashed % capacity;
	size_t coord = hash * bucket_size;

	size_t max_i = bucket_size;
	for (size_t i = 0; i < max_i; ++i, ++coord) {
		HT_Node &n = data[coord];
		if (n.is() == IS_DEAD) {
			return OK;
		}

		if (n == node) {
			n.set_is(IS_DELETED);
			return OK;
		}
	}
	return OK;
}

HT_RET_TYPE Hashtable::find(const HT_Node &node) const {
	VERIFY_T(is_valid() == OK, HT_RET_TYPE);

	size_t hash = node.hashed % capacity;
	size_t coord = hash * bucket_size;

	size_t max_i = bucket_size;
	for (size_t i = 0; i < max_i; ++i, ++coord) {
		HT_Node &n = data[coord];
		if (n.is() == IS_DEAD) {
			return (HT_RET_TYPE) 0;
		}
		
		if (n.is() == IS_ALIVE && n == node) {
			return n.val;
		}
	}

	return (HT_RET_TYPE) 0;
}

RETURN_CODE Hashtable::execute_queue(HT_Node *nodes, char *operations, size_t queue_size, HT_RET_TYPE *result) {
	assert(nodes);
	assert(operations);
	assert(result);

	for (size_t i = 0; i < 10000; ++i) {
		nodes[i].hash();
	}

	if ((long int) operations == HT_Q_INSERT) {
		for (size_t i = 0; i < queue_size; ++i, ++nodes) {
			VERIFY_OK(insert(*nodes));
		}

		return OK;
	} else if ((long int) operations == HT_Q_ERASE) {
		for (size_t i = 0; i < queue_size; ++i, ++nodes) {
			VERIFY_OK(erase(*nodes));
		}

		return OK;
	} else if ((long int) operations == HT_Q_FIND) {
		for (size_t i = 0; i < queue_size; ++i, ++nodes) {
			result[i] = find(*nodes);
		}
		return OK;
	}

	for (size_t i = 0; i < queue_size; ++i, ++nodes) {
		switch ((long int) operations[i]) {
			case HT_Q_INSERT : {
				VERIFY_OK(insert(*nodes));
				break;
			}

			case HT_Q_ERASE : {
				VERIFY_OK(erase(*nodes));
				break;
			}

			case HT_Q_FIND : {
				result[i] = find(*nodes);
				break;
			}
		}
	}

	return OK;
}


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
            pairs_cnt += *dt == '=';
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
    }

    *dict_size = pairs_cnt;

    return nodes;
}

HT_Node *prepare_random_test(HT_Node *nodes, const size_t node_cnt, const size_t test_size) {
    assert(nodes);

    HT_Node *test = (HT_Node*) calloc(test_size, sizeof(HT_Node));
    assert(test);

    for (size_t i = 0; i < test_size; ++i) {
        test[i].copy_from(nodes[rand() % node_cnt]);
    }

    return test;
}

int test_hashtable_fnv1_no_opt() {
	Hashtable ht;
	ht.ctor(10, 64);

    char *data = read_file("dict.txt");
    int dict_size = 0;
    HT_Node *nodes = cut_dict_to_nodes(data, &dict_size);

    ht.execute_queue(nodes, HT_Q_INSERT_ONLY, dict_size, HT_Q_IGNORANCE_BUFFER);

    return 0;
}

const int TEST_CNT = 10;
const int TEST_SIZE = 1000000;
int main() {
	Hashtable ht;
	ht.ctor(10, 64);

	printf("Reading dict\n");
    char *data = read_file("../dictus.txt");
    // printf("%s\n", data);
    int dict_size = 0;
    HT_Node *nodes = cut_dict_to_nodes(data, &dict_size);

    ht.execute_queue(nodes, HT_Q_INSERT_ONLY, dict_size, HT_Q_IGNORANCE_BUFFER);

    printf("Dict read, %lu pairs\n", ht.get_size());

    HT_Node *tests[TEST_CNT] = {};
    for (int i = 0; i < TEST_CNT; ++i) {
    	tests[i] = prepare_random_test(nodes, dict_size, TEST_SIZE);
    }

    const char **find_results = (const char**) calloc(TEST_SIZE, sizeof(char*));


    for (int i = 0; i < TEST_CNT; ++i) {
    	HT_Node *test = tests[i];
    	ht.execute_queue(test, HT_Q_FIND_ONLY, TEST_SIZE, find_results);
    }

    volatile HT_Node a(find_results[0], find_results[1], 1);
    a.val = find_results[3];

	return 0;
}
