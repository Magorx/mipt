#include "decision_tree.h"
#include <cstdio>

int main() {
	DecisionTree first, second;
	first.load("db1.db");
	second.load("db2.db");

	first.merge(second);

	first.save("db_out.db");

	printf(".doned.\n");
	return 0;
}