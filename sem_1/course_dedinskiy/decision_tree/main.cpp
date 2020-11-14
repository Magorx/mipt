#include "decision_tree.h"
#include <cstdio>

int main() {
	DecisionTree t;
	t.load("db.db");
	t.save("db_copy.db");

	t.save("db.db");
	
	printf("doned.\n");
	return 0;
}