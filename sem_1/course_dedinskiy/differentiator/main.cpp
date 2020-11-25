#include "expression_tree.h"

const int table_size = 257;

int main() {
	ExpressionTree tree;
	tree.ctor();
	tree.load("sample.expr");
	tree.dump();

	double table[table_size];
	table['x'] = 7;

	printf("\n");

	printf("%c = %lg\n", 'x', table['x']);

	printf("result = %lg\n", tree.evaluate(table, table_size));

	ExpressionTree *differed = tree.differentiate();
	differed->simplify();
	printf("--------------------------------------------------- differed\n");
	differed->dump();
	printf("---------------------------------------------------\n");

	return 0;
}

/*
	TODO

	deletes and memleak
*/