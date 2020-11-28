#include "expression_tree.h"

const int table_size = 257;

int main() {
	ExpressionTree tree;
	tree.ctor();
	tree.load("smpl_3.expr");
	tree.dump();
	tree.show_off("disser", SIMPLIFY);

	tree.dtor();

	return 0;
}

/*
	TODO

	deletes and memleak
*/