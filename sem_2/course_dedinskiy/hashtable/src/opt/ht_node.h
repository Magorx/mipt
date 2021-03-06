#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>

typedef const char* HT_RET_TYPE;

enum NODE_ALIVENESS {
	IS_DEAD = 0,
	IS_DELETED = 2,
	IS_ALIVE = 3,
};

const uint64_t MAX_KEY_LEN = 64;

struct HT_Node {
	char key[MAX_KEY_LEN + 1];
	const char *val;

	uint64_t hashed;
	char     is_alive;

	HT_Node(const char *string, const char *value, const int) {
		assert(string != nullptr);

		// printf("NODE %s | %s\n", string, value);

		memset (key, 0, 	 MAX_KEY_LEN + 1);
		strncpy(key, string, MAX_KEY_LEN);
		val = value;
		is_alive = IS_ALIVE;

		hashed = 0;
	}

	HT_Node(const char *string, const char *value) {
		assert(string != nullptr);

		memset (key, 0, 	 MAX_KEY_LEN + 1);
		strncpy(key, string, MAX_KEY_LEN);
		val = value;
		is_alive = IS_ALIVE;

		hashed = 0;
		hash();
	}

	HT_Node(const char *string, const int) {
		assert(string != nullptr);

		memset (key, 0, 	 MAX_KEY_LEN + 1);
		strncpy(key, string, MAX_KEY_LEN);
		val = nullptr;
		is_alive = IS_ALIVE;

		hashed = 0;
	}

	HT_Node(const char *string) {
		assert(string != nullptr);

		memset (key, 0, 	 MAX_KEY_LEN + 1);
		strncpy(key, string, MAX_KEY_LEN);
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

	uint64_t __attribute__ ((noinline)) hash();

	// uint64_t __attribute__ ((noinline)) hash() {
	// 	if (hashed) return hashed;

	// 	hashed = 14695981039346656037llu;
	// 	uint64_t prime = 1099511628211llu;
	// 	for (size_t i = 0; i < MAX_KEY_LEN; ++i) {
	// 		hashed = hashed ^ key[i];
	// 		hashed = hashed * prime;
	// 	}

	// 	return hashed;
	// }

	inline char is() const {
		return is_alive;
	}

	inline char set_is(const char new_alive) {
		return is_alive = new_alive;
	}

};

// inline bool operator==(const HT_Node &first, const HT_Node &second) {
// 	return memcmp(first.key, second.key, 32) == 0;
// }

#define RDMEMCMPJNE_0_1_RAX_RBX_L2(shift) 	\
	"mov rax, [%0 + " shift "]\n" 			\
    "mov rbx, [%1 + " shift "]\n" 			\
    "cmp rax, rbx\n"  						\
    "jne %l2\n"

inline bool operator==(const HT_Node &first, const HT_Node &second) {
    asm goto (
    	 RDMEMCMPJNE_0_1_RAX_RBX_L2("0")
         RDMEMCMPJNE_0_1_RAX_RBX_L2("8")
         RDMEMCMPJNE_0_1_RAX_RBX_L2("16")
         RDMEMCMPJNE_0_1_RAX_RBX_L2("24")
    ::"p"(first.key), "p"(second.key): "cc", "memory", "rax", "rbx": label_false);
	return 1;

label_false:
    return 0;
}

#undef RDMEMCMPJNE_0_1_RAX_RBX_L2

// inline bool operator==(const HT_Node &first, const HT_Node &second) {
//     asm goto (
//     	 "mov rax, [%0]\n"
//          "mov rbx, [%1]\n"
//          "cmp rax, rbx\n"
//          "jne %l2\n"
//          "mov rax, [%0 + 8]\n"
//          "mov rbx, [%1 + 8]\n"
//          "cmp rax, rbx\n"
//          "jne %l2\n"
//          "mov rax, [%0 + 16]\n"
//          "mov rbx, [%1 + 16]\n"
//          "cmp rax, rbx\n"
//          "jne %l2\n"
//          "mov rax, [%0 + 24]\n"
//          "mov rbx, [%1 + 24]\n"
//          "cmp rax, rbx\n"
//          "jne %l2\n"
//     ::"p"(first.key), "p"(second.key): "cc", "memory", "rax", "rbx": label_false);
// 	return 1;

// label_false:
//     return 0;
// }