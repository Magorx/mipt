#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include "general/c/common.h"
#include "general/c/strings_and_files.h"
#include "general/warnings.h"

#include <cctype>

#include "expression_node.h"
#include "latex_parts.h"

#include "parser/compiler.h"
#include "parser/general/cpp/file.hpp"

enum SHOW_OFF_WAY {
	SIMPLIFY      = 1,
	DIFFERENCIATE = 2
};

//=====================================================================================================================
// ExpressionTree =====================================================================================================


class ExpressionTree {
private:
// data =======================================================================
	ExprNode *root;
//=============================================================================

//=============================================================================
// Node disk work =============================================================
//=============================================================================

	void reading_ptr_proceed(char **buffer) {
		Char_get_next_symb(buffer);
		char *c = *buffer;
		if (*c == ')') {
			++c;
			Char_get_next_symb(&c);
		}
		*buffer = c;
	}

	void reading_ptr_skip_word(char **buffer) {
		Char_get_next_symb(buffer);
		char *c = *buffer;
		while (!isspace(*c) && *c != '(' && *c != ')') {
			++c;
		}
		*buffer = c;
	}

	ExprNode *load_node_value(char **buffer) {
		double val = 0;
		int symbs_read = 0;
		
		sscanf((const char *) *buffer, "%lg%n", &val, &symbs_read);
		if (symbs_read == 0) {
			return nullptr;
		}

		*buffer += symbs_read;
		reading_ptr_proceed(buffer);

		ExprNode *node = ExprNode::NEW(VALUE, val);
		return node;
	}

	ExprNode *load_node_variable(char **buffer) {
		char val = 0;
		int symbs_read = 0;
		
		sscanf((const char *) *buffer, "%c%n", &val, &symbs_read);
		if (symbs_read != 1) {
			return nullptr;
		}

		*buffer += symbs_read;
		reading_ptr_proceed(buffer);

		ExprNode *node = ExprNode::NEW(VARIABLE, val);
		return node;
	}

	ExprNode *load_node_operation(char **buffer) {
		char operation = 0;
		sscanf(*buffer, "%c", &operation);
		reading_ptr_skip_word(buffer);

		#define OPDEF(name, sign, arg_cnt, prior, calculation, ig1, ig2, ig3, ig4) { \
            case #sign[0]: {                                                         \
            	return ExprNode::NEW(OPERATION, #sign[0], prior);                    \
            }                                                                        \
		}

		switch (operation) {
			
			#include "ops.dsl"

			default: {
				printf("[ERR]<expr_load>: Invalid operation {%c}\n", operation);
				return nullptr;
			}
		}

		#undef OPDEF
	}

	ExprNode *load_node(char **buffer) {
		//printf("--> |%s|\n", *buffer);
		Char_get_next_symb(buffer);

		if (**buffer == ')') {
			return nullptr;
		} else if (isdigit(**buffer)) {
			return load_node_value(buffer);
		} else if ('a' <= **buffer && **buffer <= 'z') {
			return load_node_variable(buffer);
		} else { //===================================== definetly an operation
			if (!(**buffer == '(')) {
				return nullptr; //ERROR
			}
			++(*buffer);

			ExprNode *left_operand = load_node(buffer);

			if (!(**buffer)) {
				return left_operand;
			}

			if (**buffer == ')') {
				++(*buffer);
				Char_get_next_symb(buffer);
				return left_operand;
			}

			ExprNode *node = load_node_operation(buffer);
			if (!node) {return nullptr;} //ERROR

			ExprNode *right_operand = load_node(buffer);

			node->set_L(left_operand);
			node->set_R(right_operand);

			return node;
		}
	}

//=============================================================================

	void dump(ExprNode *node, int depth = 0, int to_format_cnt = 0, FILE *file_ptr = stdout) const {
		if (!node) {return;}

		dump(node->get_R(), depth + 1, to_format_cnt + 1, file_ptr);

		for (int i = 0; i < depth; ++i) {
			if (depth - to_format_cnt- 1 <= i) {
				printf("     |");
			} else {
				printf("      ");
			}
		}

		node->dump(file_ptr);
		//fprintf(file_ptr, "[%lg]", node->complexity);
		if (node->get_L() || node->get_R()) printf("->|");
		printf("\n");

		dump(node->get_L(), depth + 1, to_format_cnt + 1, file_ptr);
	}


public:
	ExpressionTree            (const ExpressionTree&) = delete;
	ExpressionTree &operator= (const ExpressionTree&) = delete;

	ExpressionTree():
	root(0)
	{}

	~ExpressionTree() {}

	void ctor() {
		root = nullptr;
	}

	static ExpressionTree *NEW() {
		ExpressionTree *cake = (ExpressionTree*) calloc(1, sizeof(ExpressionTree));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void dtor(bool to_delete_expression = true) {
		if (to_delete_expression) {
			ExprNode::DELETE(root, to_delete_expression);
		}
	}

	static void DELETE(ExpressionTree *expr_tree, bool to_delete_expression = true) {
		if (!expr_tree) {
			return;
		}

		expr_tree->dtor(to_delete_expression);
		free(expr_tree);
	}

	ExprNode *get_root() const {
		return root;
	}

	void set_root(ExprNode *root_) {
		root = root_;
	}

//=============================================================================
// Disk work ==================================================================
//=============================================================================

	int load(const char *file_name) {
#ifdef TEST_MODE
		if (file_name == nullptr) {
			printf("[ERR]<detree>: [file_name](nullptr)\n");
			return -1;
		}

		//=================================================
		Filepp input_file = {};
		input_file.ctor(file_name);
		Compiler comp = {};
		CodeNode *expr = comp.read_to_nodes(&input_file);

		FILE *tmp = fopen("tmp.tmp", "w");
		if (!tmp) {
			printf("sht\n");
			exit(0);
		}
		fprintf(tmp, "(");
		expr->space_dump(tmp);
		//fprintf(tmp, ")");
		fclose(tmp);
		//==================================================*/

		File file;
		if (File_construct(&file, "tmp.tmp") < 0) {
			printf("[ERR]<detree>: [file_name](%s) unexistance\n", file_name);
		}

		root = load_node((char**) &file.cc);
		File_destruct(&file);
		return 0;
#else
		if (file_name == nullptr) {
			printf("[ERR]<detree>: [file_name](nullptr)\n");
			return -1;
		}

		File file;
		if (File_construct(&file, file_name) < 0) {
			printf("[ERR]<detree>: [file_name](%s) unexistance\n", file_name);
		}

		root = load_node((char**) &file.cc);
		File_destruct(&file);

		return 0;
#endif
	}

	int save(const char *file_name) {
		if (file_name == nullptr) {
			printf("[ERR]<detree>: [file_name](nullptr)\n");
			return -1;
		}

		FILE *file = fopen(file_name, "w");
		if (!file) {
			printf("[ERR]<detree>: [file_name](%s) can't be opened\n", file_name);
			return -1;
		}

		//save_node(root, 0, false, file);

		fclose(file);
		return 0;
	}

//=============================================================================

	double evaluate(const double *var_table = nullptr, const size_t var_table_len = 0) {
		return root ? root->evaluate(var_table, var_table_len) : 0.0;
	}

	ExpressionTree *differentiate(FILE *file = nullptr) {
		ExpressionTree *tree = NEW();
		if (root) {
			tree->set_root(root->differentiate(file));
		}

		return tree;
	}

	char simplify_step() {
		if (!root) return 0;
		char result = 0;
		root = root->simplify_step(&result);
		return result;
	}

	void latex_dump_expression(FILE *file) {
		if (!file) return;

		fprintf(file, "$$ ");
		get_root()->latex_dump(file);
		fprintf(file, "$$\n");
	}

	void show_off(const char *file_name, const int mode) {
		if (!root) {
			printf("[ERR]<expr_swof>: [root](nullptr)\n");
			return;
		}

		if (file_name == nullptr) {
			printf("[ERR]<expr_swof>: [file_name](nullptr)\n");
			return;
		}

		FILE *file = fopen(file_name, "w");
		if (!file) {
			printf("[ERR]<expr_swof>: [file_name](%s) can't be opened\n", file_name);
			return;
		}

		fprintf(file, "%s\n", LATEX_ARTICLE_HEAD);
		fprintf(file, "%s\n", LATEX_ARTICLE_INTRODUCTION);

		latex_dump_expression(file);
		fprintf(file, "\\end{titlepage} \\newpage\n");
		fprintf(file, "\\section{\\Large{Differentiating}}\n");

		ExpressionTree *show_tree = nullptr;

		if (mode == DIFFERENCIATE) {
			show_tree = differentiate(file);
		} else {
			show_tree = NEW();
			show_tree->set_root(root->deep_copy());
		}

		fprintf(file, "And that's the answer! \\newpage \\section{\\Large{Simplifying}}\n");
		fprintf(file, "Finaly we were able to calculate difference! Let's simplify it now.\n\n");
		show_tree->latex_dump_expression(file);
		

		fprintf(file, "\\noindent\\makebox[\\linewidth]{\\rule{\\paperwidth}{0.4pt}}\n");

		char result = 0;
		while (true) {
			result = show_tree->simplify_step();
			if (!result) {
				break;
			}

			bool to_dump = true;
			switch (result) {
				case SIMPLIFIED_EVALUATIVE : {
					fprintf(file, "Some evaluations leave us with\n");
					break;
				}

				case SIMPLIFIED_ELEMENTARY : {
					fprintf(file, "No big brains are needed to get\n");
					break;
				}

				case PUT_OUT_OF_BRACKETS : {
					fprintf(file, "Sprinkling out-of-brackets magic!\n");
					break;
				}

				case REORDERED_TREE : {
					fprintf(file, "Let's reshuffle operands a bit\n");
					to_dump = true;
					break;
				}

				case FOLDED_OPERATION : {
					fprintf(file, "Here we fold in half the expression:\n");
					break;
				}

				case LINEARIZED_TREE : {
					to_dump = false;
					break;
				}

				default: {
					fprintf(file, "We are witnessing strange magic now...\n");
				}

			}

			if (to_dump) {
				show_tree->latex_dump_expression(file);
			}

			// printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
			// show_tree->dump();
			// printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
			//printf("\n");
		}

		//show_tree->set_root(show_tree->get_root()->prettify());

		fprintf(file, "%s\n", LATEX_RESULTS_AND_CONCLUSION);
		show_tree->latex_dump_expression(file);

		fprintf(file, "\\includegraphics[width=13cm, height=6cm]{welcome_to_the_internet.jpg}\n\n");

		fprintf(file, "%s\n", REFERENCES);

		fprintf(file, "\n\\end{document}");
		fclose(file);

		char generate_pdf_command[200];
		sprintf(generate_pdf_command, "pdflatex -output-directory='latex_output' -jobname='%s' %s", file_name, file_name);
		system(generate_pdf_command);

		printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
		show_tree->dump();
		printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
		//printf("===\n");
		//show_tree->root->get_L()->fold_multiplication(&a);
		//show_tree->simplify_step();
		// char ig;
		// show_tree->get_root()->fold_addition(&ig);
		// show_tree->dump();
		// printf("`````````````````````````````````````````````````\n");

		DELETE(show_tree);
	}

	void dump(FILE *file_ptr = stdout) const {
		dump(root, 0, 0, file_ptr);
	}

};

#endif // DIFFERENTIATOR