#include "expression_tree.h"

const int table_size = 257;

int main() {
	ExpressionTree tree;
	tree.ctor();
	tree.load("sample.expr");
	tree.dump();
	tree.show_off("disser");

	/*double table[table_size];
	table['x'] = 7;

	printf("\n");

	printf("%c = %lg\n", 'x', table['x']);

	printf("result = %lg\n", tree.evaluate(table, table_size));*/

	return 0;
}

/*
	TODO

	deletes and memleak
*/