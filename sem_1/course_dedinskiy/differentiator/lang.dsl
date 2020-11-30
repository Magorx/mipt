#define IS_VAL(N)  ((N) && N->type == VALUE)
#define IS_ZERO(N) ((N) && IS_VAL(N) && fabs(N->val)     < GENERAL_EPS)
#define IS_ONE(N)  ((N) && IS_VAL(N) && fabs(N->val - 1) < GENERAL_EPS)

#define IS_VAR(N) (N && N->type == VARIABLE)

#define IS_OP(N)  ((N) && (N->type) == OPERATION)
#define IS_ADD(N) ((N) && IS_OP(N) && N->cval == '+')
#define IS_SUB(N) ((N) && IS_OP(N) && N->cval == '-')
#define IS_MUL(N) ((N) && IS_OP(N) && N->cval == '*')
#define IS_DIV(N) ((N) && IS_OP(N) && N->cval == '/')
#define IS_POW(N) ((N) && IS_OP(N) && N->cval == '^')

#define NEW_ZERO() NEW(VALUE, 0, PRIOR_VALUE)
#define NEW_ONE()  NEW(VALUE, 1, PRIOR_VALUE)

#define ADD(A, B)  NEW(OPERATION, '+', PRIOR_ADD, A, B)
#define SUB(A, B)  NEW(OPERATION, '-', PRIOR_SUB, A, B)
#define MUL(A, B)  NEW(OPERATION, '*', PRIOR_MUL, A, B)
#define DIV(A, B)  NEW(OPERATION, '/', PRIOR_DIV, A, B)

#define RETURN_ZERO()  { DELETE(this, true); *result = SIMPLIFIED_ELEMENTARY; return NEW_ZERO(); }
#define RETURN_ONE()   { DELETE(this, true); *result = SIMPLIFIED_ELEMENTARY; return NEW_ONE();  }
#define RETURN_LEFT()  { ExprNode *left  = L; DELETE(R); DELETE(this); *result = SIMPLIFIED_ELEMENTARY; return left;  }
#define RETURN_RIGHT() { ExprNode *right = R; DELETE(L); DELETE(this); *result = SIMPLIFIED_ELEMENTARY; return right; }

#define RANDOM_CHANCE(x) rand() % x
#define RAND90() RANDOM_CHANCE(9)
#define RAND50() RANDOM_CHANCE(2)