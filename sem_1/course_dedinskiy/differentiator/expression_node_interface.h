#ifndef EXPRESSION_NODE_INTERFACE
#define EXPRESSION_NODE_INTERFACE

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

enum SIMPLIFICATIONS_RESULTS {
	SIMPLIFIED_EVALUATIVE = 1,
	SIMPLIFIED_ELEMENTARY = 2,
	FOLDED_OPERATION      = 3,
	PUT_OUT_OF_BRACKETS   = 4,

	REORDERED_TREE        = 10,
	LINEARIZED_TREE       = 11,
};

/*class ExprNode {
private:
// data =======================================================================
	ExprNode *L;
	ExprNode *R;

	char variable_presented;
//=============================================================================

	double evaluate_op(const double *var_table, const size_t var_table_len);
	double evaluate_variable(const double *var_table, const size_t var_table_len);

public:
// data =======================================================================
	int prior;
	int low_prior;
	int high_prior;
	char type;
	double val;
	double complexity;
//=============================================================================

	ExprNode ();
	~ExprNode();

	void update();
	void update_complexity();

	void ctor();
	void ctor(const char type_, const double val_, const int prior_ = PRIOR_VALUE, ExprNode *L_ = nullptr, ExprNode *R_ = nullptr);

	static ExprNode *NEW();
	static ExprNode *NEW(const char type_, const double val_, const int prior_ = PRIOR_VALUE, ExprNode *L_ = nullptr, ExprNode *R_ = nullptr);
	static ExprNode *NEW(const char type_, const double val_, const int prior_, ExprNode &R_);

	static void DELETE(ExprNode *exprnode, const char recursive = 0);

//=============================================================================
// get-set ====================================================================
//=============================================================================

	ExprNode *get_L() const;
	ExprNode *get_R() const;

	ExprNode *get_base();
	ExprNode *get_pow();
	ExprNode *get_coef();

	void set_pow(ExprNode *pow_);
	void set_coef(ExprNode *coef_);

	void set_L(ExprNode *L_);
	void set_R(ExprNode *R_);

	bool is_variadic();

	ExprNode *deep_copy() const;

	ExprNode &operator+(ExprNode &other);
	ExprNode &operator-(ExprNode &other);
	ExprNode &operator*(ExprNode &other);
	ExprNode &operator/(ExprNode &other);
	ExprNode &operator^(ExprNode &other);
	ExprNode &operator+(const double other);
	ExprNode &operator-(const double other);
	ExprNode &operator*(const double other);
	ExprNode &operator/(const double other);
	ExprNode &operator^(const double other);

	double evaluate(const double *var_table = nullptr, const size_t var_table_len = 0);

	ExprNode *differentiate();

	bool equivalent_absolute(const ExprNode *other) const;

	bool commutative_reorder(char op, char *success, int order = 1);
	bool commutative_linearize(char op, char *success);

	bool add(ExprNode *other);
	bool multiply(ExprNode *other);

	bool fold_addition(char *success);
	bool fold_multiplication(char *success);

	ExprNode *simplify_evaluative(char *success);
	ExprNode *simplify_elementary(char *success);
	ExprNode *simplify_structure(char *success);
	ExprNode *simplify_strange(char *success);

	ExprNode *simplify_step(char *success);

	void dump(FILE *file_ptr = stdout) const;
	void dump_space(FILE *file_ptr = stdout) const;

	void latex_dump_son(FILE *file = stdout, const ExprNode *son = nullptr) const;
	void latex_dump(FILE *file = stdout) const;

};*/

#endif // EXPRESSION_NODE_INTERFACE