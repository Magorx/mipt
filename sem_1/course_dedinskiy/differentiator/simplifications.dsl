#define NO_VARS(N) N ? !(N->is_variadic()) : 1

#define EVAL(N) N ? (double res = N->evaluate(), DELETE(N, true), NEW(VALUE, res, PRIOR_VALUE)) : 0

SMPL(VALUE, false, this)

SMPL(OPERATION, NO_VARS(L) && NO_VARS(R), )