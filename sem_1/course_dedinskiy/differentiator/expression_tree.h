#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include "general/c/common.h"
#include "general/c/strings_and_files.h"
#include "general/cpp/vector.hpp"

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
//=============================================================================

	double evaluate_operation(const double *var_table, const size_t var_table_len) {
		char operation = val;
		double L_RES  = L ? L->evaluate(var_table, var_table_len) : 0;
		double R_RES = R ? R->evaluate(var_table, var_table_len) : 0;

		#define OPDEF(name, sign, arg_cnt, prior, calculation, ig1, ig2, ig3, ig4) {                                  \
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
	int prior;
	int low_prior;
	int high_prior;
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


	bool operator<(const ExprNode &other) {
		if (type < other.type) {
			return true;
		} else if (other.type < type) {
			return false;
		}

		if (type == VALUE || type == VARIABLE) {
			return val < other.val;
		}

		if (val != '^') {
			return false;
		}

		return *R < *other.get_R();
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

	#define OPDEF(name, sign, arg_cnt, prior, calculation, differed, ig1, ig2, ig3) {                             \
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

#define IS_VAL(N) N->type == VALUE
#define IS_VAR(N) N->type == VARIABLE
#define IS_OP(N)  N->type == OPERATION
#define IS_ZERO(N) (IS_VAL(N) && fabs(N->val)     < GENERAL_EPS)
#define IS_ONE(N)  (IS_VAL(N) && fabs(N->val - 1) < GENERAL_EPS)

#define NEW_ZERO() NEW(VALUE, 0, PRIOR_VALUE)
#define NEW_ONE()  NEW(VALUE, 1, PRIOR_VALUE)

#define RETURN_ZERO()  DELETE(this, true); *success = 1; return NEW_ZERO();
#define RETURN_ONE()   DELETE(this, true); *success = 1; return NEW_ONE();
#define RETURN_LEFT()  ExprNode *left = L;  DELETE(R); DELETE(this); *success = 1; return left;
#define RETURN_RIGHT() ExprNode *right = R; DELETE(L); DELETE(this); *success = 1; return right;

#define _RANDOM_CHANCE(x) rand() % x
#define RAND90() _RANDOM_CHANCE(9)
#define RAND50() _RANDOM_CHANCE(2)

	void collect_multiplicative_cluster(Vector<ExprNode*> *buffer) {
		assert(buffer);

		if (val != '*' && val != '/') {
			buffer->push_back(this);
			return;
		}

		if (val == '*') {
			L->collect_multiplicative_cluster(buffer);
			R->collect_multiplicative_cluster(buffer);
		} else {
			L->collect_multiplicative_cluster(buffer);
		}
	}

	bool equivalent(const ExprNode &other) {
		if (type != other.type) {
			return false;
		}

		if (type == VALUE) {
			return true;
		} else {
			if (!(type == OPERATION && val == '^')) {
				return val == other.val;
			} else {
				return L->equivalent(*other.get_L());
			}
		}
	}

	ExprNode *mult_cluster_find_variadic(const Vector<char> &banned) {
		if (type == OPERATION && val == '^') {
			if (L->type == VARIABLE && !banned.contains((char) L->val)) {
				return this;
			}
		}

		if (type == OPERATION && val == '*') {
			ExprNode *ret = L->mult_cluster_find_variadic(banned);
			if (ret) return ret;
			ret = R->mult_cluster_find_variadic(banned);
			return ret;
		}

		if (type == OPERATION && val == '/') {
			return L->mult_cluster_find_variadic(banned);
		}

		if (type == VARIABLE && !banned.contains((char)val)) {
			ExprNode *left = NEW(VARIABLE, val, PRIOR_VALUE);
			type = OPERATION;
			val = '^';
			prior = PRIOR_POW;

			set_L(left);
			set_R(NEW(VALUE, 1, PRIOR_VALUE));
			return this;
		}

		return nullptr;
	}

	ExprNode *mult_cluster_find_value() {
		if (type == VALUE) {
			return this;
		}

		if (type == OPERATION && val == '*') {
			ExprNode *ret = L->mult_cluster_find_value();
			if (ret) return ret;
			ret = R->mult_cluster_find_value();
			return ret;
		}

		if (type == OPERATION && val == '/') {
			return L->mult_cluster_find_value();
		}

		return nullptr;
	}

	void suggest_multiply_val(ExprNode *node, char *success) {
		if (this == node) {
			return;
		}

		if (!node || node->type != VALUE) {
			printf("BAD VALUE NODE\n");
			return;
		}

		if (type == OPERATION && val == '*') {
			L->suggest_multiply_val(node, success);
			R->suggest_multiply_val(node, success);
		}

		if (type == OPERATION && val == '/') {
			L->suggest_multiply_val(node, success);
		}

		if (type == VALUE) {
			node->val = node->val * val;

			val = 1;
			*success = 1;
		}
	}

	void suggest_multiply_px(ExprNode *node, char *success, char to_sub = 0) {
		if (this == node) {
			return;
		}

		if (!node || node->type != OPERATION || node->val != '^') {
			printf("BAD PX NODE\n");
			return;
		}

		if (type == OPERATION && val == '*') {
			L->suggest_multiply_px(node, success, to_sub);
			R->suggest_multiply_px(node, success, to_sub);
			return;
		}

		if (type == OPERATION && val == '/') {
			L->suggest_multiply_px(node, success, to_sub);
			R->suggest_multiply_px(node, success, to_sub ^ 1);
		}

		if (type == OPERATION && val == '^') {
			if (L->type == VARIABLE && node->get_L()->val == L->val) {
				node->set_R(NEW(OPERATION, to_sub ? '-' : '+', PRIOR_ADD, R->deep_copy(), node->get_R()));
				DELETE(R, true);
				set_R(NEW(VALUE, 0, PRIOR_VALUE));
			}
		}

		if (type == VARIABLE && val == node->get_L()->val) {
			node->set_R(NEW(OPERATION, to_sub ? '-' : '+', PRIOR_ADD, NEW_ONE(), node->get_R()));

			type = VALUE;
			val = 1;
			prior = PRIOR_VALUE;
			*success = 1;
		}
	}

	ExprNode *simplify_structure(char *success) {
		if (L) L = L->simplify_structure(success);
		if (*success) {
			return this;
		}

		if (R) R = R->simplify_structure(success);
		if (*success) {
			return this;
		}

		if (!L || !R) {
			return this;
		}

		// ========================================================== operation
		switch ((char) val) {
			case '*' : {
				if (RAND50()) {
					Vector<char> x;
					x.ctor();

					ExprNode *variadic = mult_cluster_find_variadic(x);
					if (!variadic) {
						break;
					}
					ExprNode *cur_pow = variadic->R;
					suggest_multiply_px(variadic, success);
					//if (cur_pow == variadic->R) *success = 0;
					break;
				}

				if (true) {
					ExprNode *node = mult_cluster_find_value();
					if (node) {

						double cur_val = node->val;
						suggest_multiply_val(node, success);
						*success = cur_val != node->val;
					}
				}
			}
		}

		return this;
	}

	ExprNode *simplify(char *success) {
		if (L) L = L->simplify(success);
		if (*success) {
			return this;
		}

		if (R) R = R->simplify(success);
		if (*success) {
			return this;
		}

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

		switch ((char) val) {
			case '+' : {
				if (IS_ZERO(L)) {
					RETURN_RIGHT();
				}

				if (IS_ZERO(R)) {
					RETURN_LEFT();
				}

				if (IS_VAR(L) && IS_VAR(R) && L->val == R->val) {
					DELETE(R, true);
					return NEW(OPERATION, '*', PRIOR_POW, NEW(VALUE, 2, PRIOR_VALUE), L);
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

				if (IS_VAR(L) && IS_VAR(R) && L->val == R->val) {
					DELETE(R, true);
					return NEW(OPERATION, '^', PRIOR_POW, L, NEW(VALUE, 2, PRIOR_VALUE));
				}

				if (false && IS_OP(L) && L->val == '*' && IS_OP(R) && R->val == '*') {
					if (rand() % 10) {
						ExprNode *LR = L->R;
						ExprNode *RL = R->L;
						R->set_L(LR);
						L->set_R(RL);
						*success = 1;
						return this;
					}
				}

				if (false && IS_OP(L) && L->val == '*' && (IS_VAL(R) || IS_VAR(R)) && RAND90()) {
					ExprNode *LR = L->R;
					L->set_R(R);
					R = LR;

					*success = 1;
					return this;
				}

				if (false && IS_OP(R) && R->val == '*' && (IS_VAL(L) || IS_VAR(L)) && RAND90()) {
					ExprNode *RL = R->L;
					R->set_L(L);
					L = RL;

					*success = 1;
					return this;
				}

				if (false && IS_OP(L) && L->val == '/' && RAND90()) {
					ExprNode *left = L;
					set_L(left->L);
					left->set_L(this);

					*success = 1;
					return left;
				}

				break;
			}

			case '/' : {
				if (IS_ONE(R)) {
					RETURN_LEFT();
				}

				if (IS_OP(L) && L->val == '/') {
					ExprNode *prev_L = L;
					set_R(NEW(OPERATION, '*', PRIOR_MUL, R, L->R));
					set_L(L->L);
					DELETE(prev_L, false);

					*success = 1;
					return this;
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

	void latex_dump_son(FILE *file = stdout, const ExprNode *son = nullptr) const {
		if (!son) return;

		if (son->prior < prior) {
			fprintf(file, "\\left(");
		}
		son->latex_dump(file);
		if (son->prior < prior) {
			fprintf(file, "\\right)");
		}
	}

	#define OPDEF(ig1, sign, ig3, ig4, ig5, ig6, before, between, after) {       \
        case #sign[0]: {							                             \
        	fprintf(file, "%s", before);                                         \
        	latex_dump_son(file, L);                                             \
        	fprintf(file, "%s", between);                                        \
        	latex_dump_son(file, R);                                             \
        	fprintf(file, "%s", after);                                          \
        	break;                                                               \
        }                                                                        \
	}

	void latex_dump(FILE *file = stdout) const {
		//fprintf(file, "{");
		if (type == VALUE) {
			if (val < 0) {
				fprintf(file, "\\left(");
			}
			fprintf(file, "%lg", val);
			if (val < 0) {
				fprintf(file, "\\right)");
			}
		} else if (type == VARIABLE) {
			fprintf(file, "%c", (char) val);
		} else {
			switch ((char) val) {

				#include "ops.dsl"

			}
		}
		//fprintf(file, "}");
	}

	#undef OPDEF

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

	bool simplify_step() {
		if (!root) return 0;

		char success = 0;
		root = root->simplify(&success);
		return success;
	}

	bool simplify_structure_step() {
		if (!root) return 0;
		char success = 0;
		root = root->simplify_structure(&success);
		return success;
	}

	void simplify() {
		if (!root) return;
		while (simplify_step() || simplify_structure_step()) {}
	}

	void show_off(const char *file_name) {
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
			printf("[ERR]<detexpr_swofree>: [file_name](%s) can't be opened\n", file_name);
			return;
		}

		fprintf(file, "\\documentclass{article}\n");
		fprintf(file, "\\usepackage{hyperref}\n");
		fprintf(file, "\\begin{document}\n\\Large\n");
		fprintf(file, "So we are given an expression:\n");

		fprintf(file, "$$ ");
		root->latex_dump(file);
		fprintf(file, "$$\n\n");

		fprintf(file, "Let's diffirintiate it!\n");

		ExpressionTree *differed = differentiate();

		fprintf(file, "$$ ");
		differed->get_root()->latex_dump(file);
		fprintf(file, "$$\n\n");

		fprintf(file, "Uhhh, let's simplify it a bit...\n");

		char flag = 1;
		while(flag) {
			flag = 0;
			while (differed->simplify_step()) {
				flag = 1;
				fprintf(file, "SIMPLE $$ ");
				differed->get_root()->latex_dump(file);
				fprintf(file, "$$\n");
			}

			while (differed->simplify_structure_step()) {
				flag = 1;
				fprintf(file, "STRUCTURE $$ ");
				differed->get_root()->latex_dump(file);
				fprintf(file, "$$\n");
			}
			if (flag) {
			}
		}

		while(differed->simplify_step());

		fprintf(file, "So finaly:\n");
		fprintf(file, "$$ ");
		differed->get_root()->latex_dump(file);
		fprintf(file, "$$\n");


		fprintf(file, "\n\\end{document}");

		fclose(file);

		char generate_pdf_command[200];
		sprintf(generate_pdf_command, "pdflatex -output-directory='latex_output' -jobname='%s' %s", file_name, file_name);
		system(generate_pdf_command);
	}

	void dump(FILE *file_ptr = stdout) const {
		dump(root, 0, 0, file_ptr);
	}

};

#endif // DIFFERENTIATOR