#include "expression_tree.h"

const int table_size = 257;

int main() {
	int mode = SIMPLIFY;

	ExpressionTree tree;
	tree.ctor();

	if (mode == SIMPLIFY) {
		tree.load("samples/sample_simplification.expr");
	} else if (mode == DIFFERENCIATE) {
		tree.load("samples/sample_derivative.expr");
	}

	tree.dump();
	tree.show_off("disser", mode);
	tree.dtor();

	return 0;
}

/*
	TODO

	deletes and memleak
*/