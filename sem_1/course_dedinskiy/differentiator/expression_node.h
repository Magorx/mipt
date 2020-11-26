#include <cmath>

#include "general/c/common.h"
#include "general/cpp/vector.hpp"

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
	int complexity;
//=============================================================================

	ExprNode():
	L(nullptr),
	R(nullptr),
	variable_presented(0),
	type(0),
	val(0.0),
	complexity(0)
	{}

	~ExprNode() {}

	void update() {
		complexity = (L ? L->complexity : 0) + (R ? R->complexity : 0) + 1;
		variable_presented = (L ? L->variable_presented : 0) || (R ? R->variable_presented : 0) || (type == VARIABLE);
		low_prior  = min(min((L ? L->low_prior  : PRIOR_MAX), (R ? R->low_prior  : PRIOR_MAX)), prior);
		high_prior = max(max((L ? L->high_prior : PRIOR_MIN), (R ? R->high_prior : PRIOR_MIN)), prior);

		update_complexity();
	}

	void update_complexity() {
		if (type == VALUE) {
			complexity = 1;
		} else if (type == VARIABLE) {
			complexity = 2;
		} else {
			switch ((char) val) {
				case '-' :
				case '+' : {
					complexity = (L ? L->complexity : 0) + (L ? L->complexity : 0);
					break;
				}

				case '/' :
				case '*' : {
					complexity = (L ? L->complexity : 1) * (R ? R->complexity : 1);
					break;
				}

				case '^' : {
					complexity = (L ? L->complexity : 0);
					break;
				}

				default: {
					complexity = pow((L ? L->complexity : 2), (R ? R->complexity : 2));
				}
			}
		}
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

	ExprNode *get_base() {
		if (type != OPERATION) {
			return this;
		} else if (val == '^') {
			return L;
		} else if (val == '*') {
			return R;
		}
	}

	ExprNode *get_pow() {
		if (type != OPERATION || val != '^') {
			return NEW(VALUE, 1, PRIOR_VALUE);
		} else {
			return R;
		}
	}

	ExprNode *get_coef() {
		if (type != OPERATION || val != '*') {
			return NEW(VALUE, 1, PRIOR_VALUE);
		} else {
			return L;
		}
	}

	void set_pow(ExprNode *pow_) {
		if (type != OPERATION || val != '^') {
			ExprNode *left  = deep_copy();
			ExprNode *right = pow_;

			DELETE(L);
			DELETE(R);
			this->ctor(OPERATION, '^', PRIOR_POW, left, right);
		} else {
			R = pow_;
		}
	}

	void set_coef(ExprNode *coef_) {
		if (type != OPERATION || val != '*') {
			ExprNode *right  = deep_copy();
			ExprNode *left = coef_;

			DELETE(L);
			DELETE(R);
			this->ctor(OPERATION, '*', PRIOR_MUL, left, right);
		} else {
			L = coef_;
		}
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

#define IS_VAL(N)  (N->type == VALUE)
#define IS_ZERO(N) (IS_VAL(N) && fabs(N->val)     < GENERAL_EPS)
#define IS_ONE(N)  (IS_VAL(N) && fabs(N->val - 1) < GENERAL_EPS)

#define IS_VAR(N) (N->type == VARIABLE)

#define IS_OP(N)  (N->type == OPERATION)
#define IS_MUL(N) (IS_OP(N) && N->val == '*')
#define IS_POW(N) (IS_OP(N) && N->val == '^')

#define NEW_ZERO() NEW(VALUE, 0, PRIOR_VALUE)
#define NEW_ONE()  NEW(VALUE, 1, PRIOR_VALUE)

#define RETURN_ZERO()  DELETE(this, true); *success = 1; return NEW_ZERO();
#define RETURN_ONE()   DELETE(this, true); *success = 1; return NEW_ONE();
#define RETURN_LEFT()  ExprNode *left = L;  DELETE(R); DELETE(this); *success = 1; return left;
#define RETURN_RIGHT() ExprNode *right = R; DELETE(L); DELETE(this); *success = 1; return right;

#define RANDOM_CHANCE(x) rand() % x
#define RAND90() RANDOM_CHANCE(9)
#define RAND50() RANDOM_CHANCE(2)

	bool equivalent_absolute(const ExprNode *other) const {
		if (type != other->type) {
			return false;
		}

		if (type == VALUE || type == VARIABLE) {
			return fabs(val - other->val) < GENERAL_EPS;
		} else {
			if (val != other->val) {
				return false;
			} else {
				return L->equivalent_absolute(other->get_L()) && R->equivalent_absolute(other->get_R());
			}
		}
	}

	bool equivalent_multiplication(const ExprNode *other) const {
		if (type != other->type) {
			return false;
		}

		if (type == VALUE) {
			return true;
		} else if (type == VARIABLE) {
			return val == other->val;
		} else {
			if (val != other->val) {
				return false;
			}

			if (val == '^') {
				return L->equivalent_multiplication(other->get_L());
			} else {
				return L->equivalent_multiplication(other->get_L()) && R->equivalent_multiplication(other->get_R());
			}
		}
	}

	void sort_by_complexity(char operation, char *success, int order = 1) {
		if (type != OPERATION || val != operation) {
			return;
		}

		L->sort_by_complexity(operation, success, order);
		R->sort_by_complexity(operation, success, order);

		if (L->complexity * order > R->complexity * order) {
			ExprNode *r = R;
			set_R(L);
			set_L(r);
			//*success = 1;
		}
	}

	bool add(ExprNode *other) {
		ExprNode *base_a = get_base();
		ExprNode *base_b = other->get_base();
		if (!base_a->equivalent_absolute(base_b)) {
			return false;
		}

		ExprNode *coef_a = get_coef();
		ExprNode *coef_b = other->get_coef();

		set_coef(NEW(OPERATION, '+', PRIOR_ADD, coef_a, coef_b));
		other->set_coef(NEW_ZERO());

		return true;
	}

	bool multiply(ExprNode *other) {
		ExprNode *base_a = get_base();
		ExprNode *base_b = other->get_base();
		if (!base_a->equivalent_absolute(base_b)) {
			return false;
		}

		ExprNode *pow_a = get_pow();
		ExprNode *pow_b = other->get_pow();

		set_pow(NEW(OPERATION, '+', PRIOR_ADD, pow_a, pow_b));
		other->set_pow(NEW_ZERO());

		return true;
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

	ExprNode *simplify_neutral_elements(char *success) {
		printf("> ");
		dump();
		printf("\n");
		if (L) L = L->simplify_neutral_elements(success);
		if (R) R = R->simplify_neutral_elements(success);
		if (!L || !R) {
			return this;
		}

		switch((char) val) {
			case '+' : {
				if (IS_ZERO(L)) {
					RETURN_RIGHT();
				}

				if (IS_ZERO(R)) {
					RETURN_LEFT();
				}
				break;
			}

			case '-': {
				if (IS_ZERO(R)) {
					RETURN_LEFT();
				}
				break;
			}

			case '*': {
				if (IS_ONE(L)) {
					RETURN_RIGHT();
				}

				if (IS_ONE(R)) {
					RETURN_LEFT();
				}
				break;
			}

			case '/': {
				if (IS_ONE(R)) {
					RETURN_LEFT();
				}
				break;
			}

			case '^' : {
				if (IS_ONE(R)) {
					RETURN_LEFT();
				}

				if (IS_ZERO(R)) {
					RETURN_ONE();
				}
				break;
			}
		}

		return this;

	}

	void fold_multiplication(char *success) {
		printf("hi ");
		dump();
		printf("\n");
		if (!IS_MUL(this)) {
			printf("bye1\n");
			return;
		}
		if (!IS_MUL(R)) {
			printf("bye2\n");
			return;
		}

		if (R->L->multiply(L)) {
			*success = 1;
		}

		R->fold_multiplication(success);
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
			case '+' : {
				sort_by_complexity('+', success, -1);
				break;
			}

			case '*' : {
				sort_by_complexity('*', success);
				if (success) {break;}
				fold_multiplication(success);
				if (success) {break;}
			}
		}

		if (success) {
			char ig;
			return simplify_neutral_elements(&ig);;
			//return this;
		} else {
			return this;
		}
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
		if (!is_variadic()) { //todo check for bad funcs, like log
			double res = evaluate();
			DELETE(this, true);

			*success = 1;
			return NEW(VALUE, res, PRIOR_VALUE);
		}

		switch ((char) val) {
			case '+' : {
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
				break;
			}

			case '*' : {
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