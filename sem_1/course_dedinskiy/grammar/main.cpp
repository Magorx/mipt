#include <cstdlib>
#include <cstdio>

#include "general/warnings.h"

#include "general/c/announcement.h"

#include "recursive_parser.h"

int main() {
	RecursiveParser p;
	const char *expr = (const char*) "11+sin(0.5*7e15)/-3.1^x-9999;";
	printf("expr = [%s]\n", expr);
	CodeNode *ret = p.parse(expr);

	if (!ret) {
		RAISE_ERROR("Something happened!\n");
	} else {
		ret->space_dump();
		printf("\n");
	}

	printf(".doned.\n");
	return 0;
}