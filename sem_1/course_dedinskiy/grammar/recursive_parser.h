#ifndef RECURSIVE_PARSER
#define RECURSIVE_PARSER

/* Grammar ================================\

G    ::= EXPR;
EXPR ::= TERM{[+-]TERM}*
TERM ::= FACT{{/|*}FACT}*
FACT ::= -FACT | +FACT | UNIT^FACT | UNIT
UNIT ::= ID(EXPR) | ID | (EXPR) | NUMB
NUMB ::= [+-]?[0-9]+{.[0-9]+}?{[eE][0-9]+}?
ID   ::= [a-zA-Z_][a-zA-Z_0-9]*

*///=======================================/

#include "general/c/announcement.h"
#include "general/cpp/stringview.hpp"

#include <cmath>

#include "code_node.h"

enum PARSER_ERROR {
	OK = 0,

	ERROR_SYNTAX = 100,
};

//=============================================================================
// RecursiveParser ============================================================

class RecursiveParser {
private:
// data =======================================================================
	const char *expr;
	const char *init_expr_ptr;
	      int   ERROR;
	const char *ERRPOS;
//=============================================================================

	#define REQUIRE(c)                  \
		do {                            \
			if (*expr != c) {           \
				ERROR  = ERROR_SYNTAX;  \
				ERRPOS = expr;          \
				return nullptr;         \
			} else {                    \
				++expr;                 \
			}                           \
		} while (0)

	#define IF_PARSED(cur_expr, ret_name, code)            \
		CodeNode *ret_name = (code);                       \
		if (ERROR) {                                       \
			expr = cur_expr;                               \
			SET_ERR(0, init_expr_ptr);                     \
		} else

	#define SET_ERR(errcode, errpos) do {ERROR = errcode; ERRPOS = errpos;} while (0)
	#define NEXT() ++expr

	bool is_id_char(const char c) {
		return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
	}

	bool is_digit(const char c) {
		return ('0' <= c && c <= '9');
	}

	bool is_sign(const char c) {
		return c == '+' || c == '-';
	}

	bool is_multiplicative(const char c) {
		return c == '*' || c == '/';
	}

	CodeNode *parse_ID() {
		if (!is_id_char(*expr)) {
			SET_ERR(ERROR_SYNTAX, expr);
			return nullptr;
		}

		StringView *id = StringView::NEW(expr);
		size_t len = 0;
		while (is_id_char(*expr) || is_digit(*expr)) {
			++len;
			NEXT();
		}
		id->set_length(len);

		return CodeNode::NEW(ID, id);
	}

	CodeNode *parse_NUMB() {
		if (!(is_digit(*expr) || is_sign(*expr))) {
			SET_ERR(ERROR_SYNTAX, expr);
			return nullptr;
		}

		char sign = '+';
		if (is_sign(*expr)) {
			sign = *expr;
			NEXT();
		}

		if (!is_digit(*expr)) {
			SET_ERR(ERROR_SYNTAX, expr);
			return nullptr;
		}

		double val = 0;
		while (is_digit(*expr)) {
			val = val * 10 + (*expr - '0');
			NEXT();
		}

		if (*expr == '.') {
			NEXT();
			if (!is_digit(*expr)) {
				SET_ERR(ERROR_SYNTAX, expr);
				return nullptr;
			}

			double frac = 0;
			int len_frac = 1;
			while (is_digit(*expr)) {
				len_frac *= 10;
				frac = frac * 10 + (*expr - '0');
				NEXT();
			}

			val = val + frac / (len_frac);
		}

		if (*expr == 'e' || *expr == 'E') {
			NEXT();

			char exp_sign = '+';
			if (is_sign(*expr)) {
				exp_sign = *expr;
				NEXT();
			}

			if (!is_digit(*expr)) {
				SET_ERR(ERROR_SYNTAX, expr);
				return nullptr;
			}

			double ten_pow = 0;
			while (is_digit(*expr)) {
				ten_pow = ten_pow * 10 + (*expr - '0');
				NEXT();
			}

			if (exp_sign == '-') {
				ten_pow *= -1;
			}

			val = val * pow(10, ten_pow);
		}

		if (sign == '-') {
			val *= -1;
		}

		return CodeNode::NEW(VALUE, val);
	}

	CodeNode *parse_UNIT() {
		IF_PARSED (expr, unit_id, parse_ID()) {
			if (*expr == '(') {
				NEXT();
				IF_PARSED (expr, bracket_expr, parse_EXPR()) {
					if (*expr == ')') {
						NEXT();
						unit_id->set_R(bracket_expr);
						return unit_id;
					}
				}
				NEXT();
				SET_ERR(ERROR_SYNTAX, expr);
				CodeNode::DELETE(unit_id);
				return nullptr;
			} else {
				return unit_id;
			}
		}

		if (*expr == '(') {
			NEXT();
			IF_PARSED (expr, expr_in_brackets, parse_EXPR()) {
				if (*expr == ')') {
					NEXT();
					return expr_in_brackets;
				} else {
					CodeNode::DELETE(expr_in_brackets);
				}
			}
			NEXT();
			SET_ERR(ERROR_SYNTAX, expr);
			return nullptr;
		}

		IF_PARSED (expr, numb, parse_NUMB()) {
			return numb;
		}

		SET_ERR(ERROR_SYNTAX, expr);
		return nullptr;
	}

	CodeNode *parse_FACT() {
		if (is_sign(*expr)) {
			char sign = *expr;
			NEXT();
			IF_PARSED (expr, fact, parse_FACT()) {
				return CodeNode::NEW(OPERATION, sign, nullptr, fact);
			}
			SET_ERR(ERROR_SYNTAX, expr);
			return nullptr;
		}

		IF_PARSED (expr, unit, parse_UNIT()) {
			if (*expr == '^') {
				NEXT();
				IF_PARSED (expr, fact, parse_FACT()) {
					return CodeNode::NEW(OPERATION, '^', unit, fact);
				}
				CodeNode::DELETE(unit);
				SET_ERR(ERROR_SYNTAX, expr);
				return nullptr;
			} else {
				return unit;
			}
		}
		
		SET_ERR(ERROR_SYNTAX, expr);
		return nullptr;
	}

	CodeNode *parse_TERM() {
		IF_PARSED (expr, fact, parse_FACT()) {
			while (is_multiplicative(*expr)) {
				char op = *expr;
				NEXT();

				IF_PARSED (expr, next_fact, parse_FACT()) {
					fact = CodeNode::NEW(OPERATION, op, fact, next_fact);
					continue;
				}

				CodeNode::DELETE(fact, true);
				SET_ERR(ERROR_SYNTAX, expr);
				return nullptr;
			}

			return fact;
		}

		SET_ERR(ERROR_SYNTAX, expr);
		return nullptr;
	}

	CodeNode *parse_EXPR() {
		IF_PARSED (expr, term, parse_TERM()) {
			while (is_sign(*expr)) {
				char op = *expr;
				NEXT();

				IF_PARSED (expr, next_term, parse_TERM()) {
					term = CodeNode::NEW(OPERATION, op, term, next_term);
					continue;
				}

				CodeNode::DELETE(term, true);
				SET_ERR(ERROR_SYNTAX, expr);
				return nullptr;
			}

			return term;
		}

		SET_ERR(ERROR_SYNTAX, expr);
		return nullptr;
	}       

	CodeNode *parse_G() {

		IF_PARSED (expr, ret, parse_EXPR()) {
			if (*expr == ';') {
				NEXT();
				return ret;
			} else {
				NEXT();
				CodeNode::DELETE(ret);
			}
		}

		SET_ERR(ERROR_SYNTAX, expr);

		return nullptr;
	}

public:
	RecursiveParser            (const RecursiveParser&) = delete;
	RecursiveParser &operator= (const RecursiveParser&) = delete;

	RecursiveParser():
	expr(nullptr),
	init_expr_ptr(nullptr),
	ERROR(0),
	ERRPOS(nullptr)
	{}

	~RecursiveParser() {}

	void ctor() {
		expr = nullptr;
		init_expr_ptr = nullptr;
		ERROR = 0;
	}

	static RecursiveParser *NEW() {
		RecursiveParser *cake = (RecursiveParser*) calloc(1, sizeof(RecursiveParser));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void dtor() {
		expr = nullptr;
		init_expr_ptr = nullptr;
		ERROR = 0;
	}

	static void DELETE(RecursiveParser *classname) {
		if (!classname) {
			return;
		}

		classname->dtor();
		free(classname);
	}

//=============================================================================

	CodeNode *parse(const char *expression) {
		expr = expression;
		init_expr_ptr = expression;

		CodeNode *res = parse_G();
		if (!ERROR) {
			return res;
		} else {
			RAISE_ERROR_SYNTAX(init_expr_ptr, ERRPOS - init_expr_ptr);
			return nullptr;
		}
	}

};

#endif // RECURSIVE_PARSER