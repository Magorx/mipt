#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include "general/c/common.h"
#include "general/c/strings_and_files.h"

#include <cctype>

template <typename T>
const T &min(const T &first, const T &second) {
	return first < second ? first : second;
}

template <typename T>
const T &max(const T &first, const T &second) {
	return second < first ? first : second;
}

enum EXPRNODE_TYPES {
	OPERATION = 1,
	VALUE     = 2,
	VARIABLE  = 3,
};

enum PRIORITIES {
	PRIOR_MAX   = 999,
	PRIOR_MIN   = 0,
	PRIOR_VALUE = 10,

	PRIOR_ADD  = 5,
	PRIOR_SUB  = 5,
	PRIOR_MUL  = 7,
	PRIOR_DIV  = 7,
}

//=============================================================================
// ExprNode ===================================================================

class ExprNode {
private:
// data =======================================================================
	char type;
	double val;

	ExprNode *L;
	ExprNode *R;

	size_t complexity;
	char variable_presented;

	int prior;
	int low_prior;
	int high_prior;
//=============================================================================

	double evaluate_operation(const double *var_table, const size_t var_table_len) {
		VERIFY_T(L && R, double);

		char operation = val;
		switch (operation) {
			case '+': {
				return L->evaluate() + R->evaluate();
			}

			case '-': {
				return L->evaluate() - R->evaluate();;
			}

			case '*': {
				return L->evaluate() * R->evaluate();;
			}

			case '/': {
				return L->evaluate() / R->evaluate();;
			}

			default: {
				VERIFY_T("INCORRECT_OPERATION" == OK, double);
				return 0;
			}
		}
	}

	double evaluate_variable(const double *var_table, const size_t var_table_len) {
		char var = value;
		if (var_table_len <= (size_t) var) {
			return (double) KCTF_POISON;
		} else  {
			return var_table[var];
		}
	}


public:
	ExprNode():
	type(0),
	val(0.0),
	L(nullptr),
	R(nullptr),
	complexity(0),
	variable_presented(0)
	{}

	~ExprNode() {}

	void update() {
		complexity = (L ? L->complexity : 0) + (R ? R->complexity : 0) + 1;
		variable_presented = (L ? L->variable_presented : 0) || (R ? R->variable_presented : 0) || (type == VARIABLE);
		low_prior  = min(min((L ? L->low_prior  : PRIOR_MAX), (R ? R->low_prior  : PRIOR_MAX)), prior);
		high_prior = max(max((L ? L->high_prior : PRIOR_MIN), (R ? R->high_prior : PRIOR_MIN)), prior);
	}

	void ctor() {
		type = 0;
		val  = 0;
		
		L = nullptr;
		R = nullptr;

		complexity = 0;
		variable_presented = 0;
	}

	static ExprNode *NEW() {
		ExprNode *cake = calloc(1, sizeof(ExprNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void ctor(const char type_, const double val_, const int prior_ = PRIOR_VALUE, ExprNode *L_ = nullptr, ExprNode *R_ = nullptr) {
		type = type_;
		val  = val_;
		
		L = L_;
		R = R_;

		if (type == VARIABLE) {
			variable_presented = 1;
		}

		prior = prior_;

		update();
	}

	static ExprNode *NEW(const char type_, const double val_, const int prior_ = PRIOR_VALUE, ExprNode *L_ = nullptr, ExprNode *R_ = nullptr) {
		ExprNode *cake = calloc(1, sizeof(ExprNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor(type_, val_, L_, R_);
		return cake;
	}

	void dtor(const char recursive = 0) {
		type = 0;
		val  = 0;

		if (recursive) {
			if (L) L->dtor(recursive);
			if (R) R->dtor(recursive);
		}

		L = nullptr;
		R = nullptr;

		update();
	}

	static void DELETE(ExprNode *exprnode, const char recursive = 0) {
		if (!exprnode) {
			return;
		}

		if (recursive) {
			DELETE(L, recursive);
			DELETE(R, recursive);
		}

		exprnode->dtor();
		free(exprnode);
	}

//=============================================================================
// Maths ======================================================================
//=============================================================================

	double evaluate(const double *var_table, const size_t var_table_len) {
		double res = 0.0;

		switch(type) {
			case (OPERATION) : {
				return evaluate_operation(var_table, var_table_len);
			}

			case (VARIABLE) : {
				return evaluate_variable (var_table, var_table_len);
			}

			case (VALUE) : {
				return val;
			}
		}
	}

//=============================================================================

};

//=============================================================================
// ExpressionTree =============================================================

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
		while (*c == ')') {
			++c;
			Char_get_next_symb(&c);
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
		++buffer;
		reading_ptr_proceed(buffer);

		switch (operation) {
			case '+' : {
				return ExprNode::NEW(OPERATION, operation, PRIOR_ADD);
			}

			case '-' : {
				return ExprNode::NEW(OPERATION, operation, PRIOR_SUB);
			}

			case '*' : {
				return ExprNode::NEW(OPERATION, operation, PRIOR_MUL);
			}

			case '/' : {
				return ExprNode::NEW(OPERATION, operation, PRIOR_SUB);
			}

			default: {
				return nullptr;
			}
		}
	}

	ExprNode *load_node(char **buffer) {
		reading_ptr_proceed(buffer);

		if (**buffer == ')') {
			return nullptr;
		} else if (isdigit(**buffer)) {
			return load_node_value(buffer);
		} else if ('a' <= **buffer && **buffer <= 'z') {
			return load_node_variable(buffer);
		} else { //===================================== definetly an operation
			if (!(*c == '(')) {
				return nullptr; //ERROR
			}

			ExprNode *left_operand = load_node(buffer);

			ExprNode *node = load_node_operation(buffer);
			if (!node) {return nullptr;} //ERROR

			ExprNode *right_operand = load_node(buffer);

			node->L = left_operand;
			node->R = right_operand;

			return node;
		}
	}

//=============================================================================


public:
	ExpressionTree():
	{}

	~ExpressionTree() {}

	void ctor() {

	}

	static ExpressionTree *NEW() {
		ExpressionTree *cake = calloc(1, sizeof(ExpressionTree));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void dtor() {

	}

	static void DELETE(ExpressionTree *classname) {
		if (!classname) {
			return;
		}

		classname->dtor();
		free(classname);
	}

	ExprNode *get_root() const {
		return root;
	}

//=============================================================================
// Disk work ==================================================================
//=============================================================================

	int load(const char *file_name) {
		if (file_name == nullptr) {
			printf("[ERR]<detree>: [file_name](nullptr)\n");
			return -1;
		}

		File *file = (File*) calloc(1, sizeof(File));
		if (File_construct(file, file_name) < 0) {
			printf("[ERR]<detree>: [file_name](%s) unexistance\n", file_name);
		}

		root = load_node(&file->cc);
		db_file = file;

		return 0;
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

		save_node(root, 0, false, file);

		fclose(file);
		return 0;
	}

//=============================================================================

};

#endif // DIFFERENTIATOR