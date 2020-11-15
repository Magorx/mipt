#include "decision_tree.h"
#include <cstdio>

int main() {
	/* merge 
	DecisionTree first, second;
	first.load("db1.db");
	second.load("db2.db");

	first.merge(second);

	first.save("db_out.db");*/

	DecisionTree tree;
	tree.load("db.db");

	tree.run_guess();
	tree.save("db_out.db");

	printf(".doned.\n");
	return 0;
}