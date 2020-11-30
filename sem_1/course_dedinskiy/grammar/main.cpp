#include <cstdlib>
#include <cstdio>

#include "general/c/announcement.h"

#define NEXT() do {++s;} while(0)
#define SYNTAX_ERROR() do {printf("You're son of a beach...\n"); exit(0);} while(0)
#define REQUIRE(c) do {if (*s != c) {SYNTAX_ERROR();} else NEXT();} while(0)

const char *s = nullptr;

int get_N() {
	if (!('0' <= *s && *s <= '9')) {
		SYNTAX_ERROR();
	}

	int val = 0;
	while ('0' <= *s && *s <= '9') {
		val = val * 10 + *s - '0';
		NEXT();
	}

	return val;
}

int get_G(const char *str) {
	s = str;
	int val = get_N();
	REQUIRE('\0');
	return val;
}


int main() {
	int val = get_G("15");
	printf("val = %d\n", val);

	char *code = (char*) "abcdefghi@klmnopqrstuvwxyz";
	RAISE_ERROR_SYNTAX(code, 1);
	RAISE_ERROR_SYNTAX(code, 5);
	RAISE_ERROR_SYNTAX(code, 10);

	printf(".doned.\n");
	return 0;
}