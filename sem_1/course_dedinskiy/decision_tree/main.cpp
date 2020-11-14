#include "decision_tree.h"
#include <cstdio>

int main() {
	DecisionTree t;
	t.load("db.db");
	t.save("db_runtime_copy.db");

	t.run_guess();

	t.save("db.db");
	
	printf(".doned.\n");
	return 0;
}