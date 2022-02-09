OPERATOR_DEF_BINARY_SAME(+, add)
OPERATOR_DEF_BINARY_SAME(-, sub)
OPERATOR_DEF_BINARY_SAME(*, mul)
OPERATOR_DEF_BINARY_SAME(/, divide)
OPERATOR_DEF_BINARY_SAME(%, mod)

OPERATOR_DEF_UNARY_PREF_SAME(+, un_add)
OPERATOR_DEF_UNARY_PREF_SAME(-, un_sub)

OPERATOR_DEF_UNARY_PREF_SAME_REF(++, pref_add)
OPERATOR_DEF_UNARY_PREF_SAME_REF(--, pref_sub)
OPERATOR_DEF_UNARY_POST_SAME(++, post_add)
OPERATOR_DEF_UNARY_POST_SAME(--, post_sub)

OPERATOR_DEF_BINARY_TYPE(bool, ==, eq)
OPERATOR_DEF_BINARY_TYPE(bool, !=, ne)
OPERATOR_DEF_BINARY_TYPE(bool, <,  lt)
OPERATOR_DEF_BINARY_TYPE(bool, <=, le)
OPERATOR_DEF_BINARY_TYPE(bool, >,  gt)
OPERATOR_DEF_BINARY_TYPE(bool, >=, ge)

OPERATOR_DEF_UNARY_PREF_SAME(~, b_not)
OPERATOR_DEF_BINARY_SAME(&,  b_and)
OPERATOR_DEF_BINARY_SAME(|,  b_or)
OPERATOR_DEF_BINARY_SAME(^,  b_xor)
OPERATOR_DEF_BINARY_SAME(<<, b_ls)
OPERATOR_DEF_BINARY_SAME(>>, b_rs)

OPERATOR_DEF_UNARY_PREF_SAME(!, l_neg)
OPERATOR_DEF_BINARY_TYPE(bool, &&, l_and)
OPERATOR_DEF_BINARY_TYPE(bool, ||, l_or)

OPERATOR_DEF_INPLACE(+=, asgn_add)
OPERATOR_DEF_INPLACE(-=, asgn_sub)
OPERATOR_DEF_INPLACE(*=, asgn_mul)
OPERATOR_DEF_INPLACE(/=, asgn_div)
OPERATOR_DEF_INPLACE(%=, asgn_mod)
OPERATOR_DEF_INPLACE(&=, asgn_b_and)
OPERATOR_DEF_INPLACE(|=, asgn_b_or)
OPERATOR_DEF_INPLACE(^=, asgn_b_xor)
OPERATOR_DEF_INPLACE(<=, asgn_b_sl)
OPERATOR_DEF_INPLACE(>=, asgn_b_sr)
