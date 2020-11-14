#include "decision_tree.h"
#include <cstdio>

int main() {
	DecisionTree t;
	t.load("dt.db");
	t.dump();
	
	printf("doned.\n");
	return 0;
}