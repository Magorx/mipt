#include <cstdlib>
#include <cstdio>

#include "general/constants.h"
#include "general/warnings.h"

#include "general/c/announcement.h"

#include "compiler.h"

int main() {
	Compiler comp = {};
	comp.read_to_nodes("prog.wzr");

	return 0;

	RecursiveParser p = {};
	const char *expr = (const char*) "x = (((a = 5) + (b = 7)) * 0 + (a = a + b^2) * 0) * 0 + (a + b * sin(15 + -a^+b));";
	printf("%s\n    |\n    Y\n", expr);
	CodeNode *ret = p.parse(expr);

	if (!ret) {
		//RAISE_ERROR("Something happened!\n");
	} else {
		//ret->space_dump();
		//printf("\n");
		const int var_tabe_size = 257;
		double var_table[var_tabe_size];
		for (int i = 0; i < var_tabe_size; ++i) {
			var_table[i] = KCTF_POISON;
		}

		//================== vars here
		var_table['x'] = 4;
		var_table['y'] = 3;
		var_table['z'] = 19;
		//================== vars here

		printf("+---+------------+\n");
		for (int i = 0; i < var_tabe_size; ++i) {
			if (fabs(var_table[i] - KCTF_POISON) > GENERAL_EPS) {
				printf("| %c | % 11lf|\n", i, var_table[i]);
			}
		}
		printf("+---+------------+\n");
		printf("    |\n    Y\n");

		printf("res = [%10lf]\n", ret->evaluate_expr(var_table, var_tabe_size));
		printf("+---+------------+\n");
		for (int i = 0; i < var_tabe_size; ++i) {
			if (fabs(var_table[i] - KCTF_POISON) > GENERAL_EPS) {
				printf("| %c | % 11lf|\n", i, var_table[i]);
			}
		}
		printf("+---+------------+\n");
	}

	printf(".doned.\n");
	return 0;
}