#include "expression_tree.h"

const int table_size = 257;

//#define TEST_MODE

int stress();

int main() {
	int mode = SIMPLIFY;

	ExpressionTree tree = {};
	tree.ctor();

	if (mode == SIMPLIFY) {
		tree.load("samples/sample_simplification.expr");
	} else if (mode == DIFFERENCIATE) {
		tree.load("samples/sample_derivative.expr");
	}

	tree.dump();
	tree.show_off("tmp", mode);
	tree.dtor();

	return 0;
}
















int stress() {
	int mode = DIFFERENCIATE;

	ExpressionTree tree;
	tree.ctor();

	if (mode == SIMPLIFY) {
		tree.load("samples/sample_simplification.expr");
	} else if (mode == DIFFERENCIATE) {
		tree.load("samples/sample_derivative.expr");
	}

	tree.dump();
	tree.show_off("tmp", mode);

	ExpressionTree *dif = tree.differentiate();
	char x = 1;
	for (int i = 2; i <= 6; ++i) {
		ExpressionTree *new_dif = dif->differentiate();

		while(x) {
			x = 0;
			new_dif->set_root(new_dif->get_root()->simplify_elementary(&x));
		}
		x = 1;
		printf("%d done\n", i);
		ExpressionTree::DELETE(dif);
		dif = new_dif;
		//dif->dump();
	}
	//tree.dump();
	//tree.show_off("disser", mode);
	tree.dtor();

	return 0;
}

/*
	TODO

	deletes and memleak
*/