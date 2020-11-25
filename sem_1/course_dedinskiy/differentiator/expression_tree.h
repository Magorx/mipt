#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include "general/c/common.h"
#include "general/c/strings_and_files.h"

#include <cctype>
#include <cmath>

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
	PRIOR_VALUE = 15,

	PRIOR_ADD  = 5,
	PRIOR_SUB  = 5,
	PRIOR_MUL  = 7,
	PRIOR_DIV  = 7,

	PRIOR_POW  = 9,
	PRIOR_SIN  = 15,
	PRIOR_COS  = 15,
	PRIOR_LOG  = 10,
};

//=============================================================================
// ExprNode ===================================================================

class ExprNode {
private:
// data =======================================================================
	ExprNode *L;
	ExprNode *R;

	size_t complexity;
	char variable_presented;

	int prior;
	int low_prior;
	int high_prior;
//=============================================================================

	double evaluate_operation(const double *var_table, const size_t var_table_len) {
		char operation = val;
		double L_RES  = L ? L->evaluate(var_table, var_table_len) : 0;
		double R_RES = R ? R->evaluate(var_table, var_table_len) : 0;

		#define OPDEF(name, sign, arg_cnt, prior, calculation, ign) {                                                 \
            case #sign[0]: {                                                                                          \
            	if (!((arg_cnt == 2 && L && R) || (arg_cnt == 1 && !L && R) || (arg_cnt == 0 && !L && !R))) {         \
        			printf("[ERR]<exrp_eval>: Operation {%c} has bad number of args, need %d\n", #sign[0], arg_cnt);  \
        			return 0;                                                                                         \
            	}                                                                                                     \
                                                                                                                      \
            	return calculation;                                                                                   \
            }                                                                                                         \
		}

		switch (operation) {

			#include "ops.dsl"

			default: {
				printf("[ERR]<expr_eval>: Invalid operation {%c}\n", operation);
				return 0;
			}
		}

		#undef OPDEF
	}

	double evaluate_variable(const double *var_table, const size_t var_table_len) {
		int var = val;
		if (var_table_len <= (size_t) var) {
			return (double) KCTF_POISON;
		} else  {
			return var_table[var];
		}
	}


public:
// data =======================================================================
	char type;
	double val;
//=============================================================================

	ExprNode():
	L(nullptr),
	R(nullptr),
	complexity(0),
	variable_presented(0),
	type(0),
	val(0.0)
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
		ExprNode *cake = (ExprNode*) calloc(1, sizeof(ExprNode));
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
		ExprNode *cake = (ExprNode*) calloc(1, sizeof(ExprNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor(type_, val_, prior_, L_, R_);
		return cake;
	}

	static ExprNode *NEW(const char type_, const double val_, const int prior_, ExprNode &R_) {
		return NEW(type_, val_, prior_, nullptr, &R_);
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
			DELETE(exprnode->L, recursive);
			DELETE(exprnode->R, recursive);
		}

		exprnode->dtor();
		free(exprnode);
	}

//=============================================================================
// get-set ====================================================================
//=============================================================================

	ExprNode *get_L() const {
		return L;
	}

	ExprNode *get_R() const {
		return R;
	}

	void set_L(ExprNode *L_) {
		L = L_;
		update();
	}

	void set_R(ExprNode *R_) {
		R = R_;
		update();
	}

	bool is_variadic() {
		return variable_presented;
	}

	ExprNode *deep_copy() const {
		ExprNode *node = NEW(type, val, prior);
		if (L) node->set_L(L->deep_copy());
		if (R) node->set_R(R->deep_copy());

		node->update();
		return node;
	}

//=============================================================================
// Operations with nodes ======================================================
//=============================================================================
	
	ExprNode &operator+(ExprNode &other) {
		return *NEW(OPERATION, '+', PRIOR_ADD, this, &other);
	}

	ExprNode &operator-(ExprNode &other) {
		return *NEW(OPERATION, '-', PRIOR_SUB, this, &other);
	}

	ExprNode &operator*(ExprNode &other) {
		return *NEW(OPERATION, '*', PRIOR_MUL, this, &other);
	}

	ExprNode &operator/(ExprNode &other) {
		return *NEW(OPERATION, '/', PRIOR_DIV, this, &other);
	}

	ExprNode &operator^(ExprNode &other) {
		return *NEW(OPERATION, '^', PRIOR_POW, this, &other);
	}

	ExprNode &operator+(const double other) {
		return *NEW(OPERATION, '+', PRIOR_ADD, this, NEW(VALUE, other, PRIOR_VALUE));
	}

	ExprNode &operator-(const double other) {
		return *NEW(OPERATION, '-', PRIOR_SUB, this, NEW(VALUE, other, PRIOR_VALUE));
	}

	ExprNode &operator*(const double other) {
		return *NEW(OPERATION, '*', PRIOR_MUL, this, NEW(VALUE, other, PRIOR_VALUE));
	}

	ExprNode &operator/(const double other) {
		return *NEW(OPERATION, '/', PRIOR_DIV, this, NEW(VALUE, other, PRIOR_VALUE));
	}

	ExprNode &operator^(const double other) {
		return *NEW(OPERATION, '^', PRIOR_POW, this, NEW(VALUE, other, PRIOR_VALUE));
	}

//=============================================================================
// Maths ======================================================================
//=============================================================================

	double evaluate(const double *var_table = nullptr, const size_t var_table_len = 0) {
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

			default: {
				printf("BAD EXPR\n");
				return 0.0;
			}
		}
	}

	#define OPDEF(name, sign, arg_cnt, prior, calculation, differed) {                                            \
        case #sign[0]: {                                                                                          \
        	if (!((arg_cnt == 2 && L && R) || (arg_cnt == 1 && !L && R) || (arg_cnt == 0 && !L && !R))) {         \
    			printf("[ERR]<exrp_eval>: Operation {%c} has bad number of args, need %d\n", #sign[0], arg_cnt);  \
    			return 0;                                                                                         \
        	}                                                                                                     \
                                                                                                                  \
        	return &(differed);                                                                                   \
        }                                                                                                         \
	}

	ExprNode *differentiate() {
		if (type == VALUE) {
			return NEW(VALUE, 0, PRIOR_VALUE);
		} else if (type == VARIABLE) {
			return NEW(VALUE, 1, PRIOR_VALUE);
		} else {
			switch ((char) val) {

				#include "ops.dsl"

				default: {
					printf("[ERR]<expr_diff>: Invalid operation {%c}\n", (char) val);
					return 0;
				}
			}
		}
	}

	#undef OPDEF

	ExprNode *simplify(char *success) {
		if (L) L = L->simplify(success);
		if (R) R = R->simplify(success);

		if (!L || !R) {
			return this;
		}

		// ========================================================== operation
		if (!is_variadic()) {
			double res = evaluate();
			DELETE(this, true);

			*success = 1;
			return NEW(VALUE, res, PRIOR_VALUE);
		}

		#define IS_VAL(N) N->type == VALUE
		#define IS_ZERO(N) (IS_VAL(N) && fabs(N->val)     < GENERAL_EPS)
		#define IS_ONE(N)  (IS_VAL(N) && fabs(N->val - 1) < GENERAL_EPS)

		#define NEW_ZERO() NEW(VALUE, 0, PRIOR_VALUE)
		#define NEW_ONE()  NEW(VALUE, 1, PRIOR_VALUE)

		#define RETURN_ZERO()  DELETE(this, true); *success = 1; return NEW_ZERO();
		#define RETURN_ONE()   DELETE(this, true); *success = 1; return NEW_ONE();
		#define RETURN_LEFT()  ExprNode *left = L;  DELETE(R); DELETE(this); *success = 1; return left;
		#define RETURN_RIGHT() ExprNode *right = R; DELETE(L); DELETE(this); *success = 1; return right;

		switch ((char) val) {
			case '+' : {
				if (IS_ZERO(L)) {
					RETURN_RIGHT();
				}

				if (IS_ZERO(R)) {
					RETURN_LEFT();
				}
				break;
			}

			case '-' : {
				if (IS_ZERO(L)) {
					val = '*';
					prior = PRIOR_MUL;
					L->val = -1;

					*success = 1;
					return this;
				}

				if (IS_ZERO(R)) {
					RETURN_LEFT();
				}
				break;
			}

			case '*' : {
				if (IS_ONE(L)) {
					RETURN_RIGHT();
				}

				if (IS_ONE(R)) {
					RETURN_LEFT();
				}

				if (IS_ZERO(L) || IS_ZERO(R)) {
					RETURN_ZERO();
				}
				break;
			}

			case '/' : {
				if (IS_ONE(R)) {
					RETURN_LEFT();
				}
				break;
			}

			case '^' : {
				if (IS_ZERO(R)) {
					RETURN_ONE();
				}

				if (IS_ONE(R)) {
					RETURN_LEFT();
				}

				if (IS_ZERO(L) && IS_VAL(R)) {
					RETURN_ZERO();
				}
				break;
			}

			default: {
				return this;
			}
		}

		return this;
	}

//=============================================================================

	void dump(FILE *file_ptr = stdout) const {
		if (type == VALUE) {
			fprintf(file_ptr, "%03lg", val);
		} else {
			fprintf(file_ptr, "{%c}", (char) val);
		}
	}

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

		#define OPDEF(name, sign, arg_cnt, prior, calculation, ign) { \
            case #sign[0]: {                                          \
            	return ExprNode::NEW(OPERATION, #sign[0], prior);     \
            }                                                         \
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

		dump(node->get_L(), depth + 1, to_format_cnt + 1, file_ptr);

		for (int i = 0; i < depth; ++i) {
			if (depth - to_format_cnt- 1 <= i) {
				printf("     |");
			} else {
				printf("      ");
			}
		}

		node->dump(file_ptr);
		if (node->get_L() || node->get_R()) printf("->|");
		printf("\n");

		dump(node->get_R(), depth + 1, to_format_cnt + 1, file_ptr);
	}


public:
	ExpressionTree():
	root(0)
	{}

	~ExpressionTree() {}

	void ctor() {

	}

	static ExpressionTree *NEW() {
		ExpressionTree *cake = (ExpressionTree*) calloc(1, sizeof(ExpressionTree));
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

	void set_root(ExprNode *root_) {
		root = root_;
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

		root = load_node((char**) &file->cc);
		//db_file = file;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11!!!!!!!!!!!!!!!!!!!!!!!! DELETE FILE

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

		//save_node(root, 0, false, file);

		fclose(file);
		return 0;
	}

//=============================================================================

	double evaluate(const double *var_table = nullptr, const size_t var_table_len = 0) {
		return root ? root->evaluate(var_table, var_table_len) : 0.0;
	}

	ExpressionTree *differentiate() {
		ExpressionTree *tree = NEW();
		if (root) {
			tree->set_root(root->differentiate());
		}

		return tree;
	}

	void simplify() {
		if (!root) return;

		char success = 1;
		while (success) {
			success = 0;
			root = root->simplify(&success);
		}
	}

	void dump(FILE *file_ptr = stdout) const {
		dump(root, 0, 0, file_ptr);
	}

};

#endif // DIFFERENTIATOR