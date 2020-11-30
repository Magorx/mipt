#ifndef RECURSIVE_PARSER
#define RECURSIVE_PARSER

/* Grammar =============================\

G    ::= EXPR'\0'
EXPR ::= TERM{[+-]TERM}*
TERM ::= FACT{[/*]FACT}*
FACT ::= UNIT^FACT | UNIT | -FACT | +FACT
UNIT ::= NUMB | (EXPR) | FUNC(EXPR)
NUMB ::= [+-]?[0-9]+{.[0-9]+}?
FUNC ::= [a-zA-Z_]+

*///====================================/

#include "general/c/announcement.h"

//=============================================================================
// ClassName ==================================================================

class ClassName {
private:
// data =======================================================================
	char *expr;
	char *ini_expr_ptr;
	int ERROR;
	int ERRPOS;
//=============================================================================

	double get_G() {

	}

public:
	ClassName():
	expr(nullptr),
	ini_expr_ptr(nullptr),
	ERROR(0)
	{}

	~ClassName() {}

	void ctor() {
		expr = nullptr;
		ini_expr_ptr = nullptr;
		ERROR = 0;
	}

	static ClassName *NEW() {
		ClassName *cake = (ClassName*) calloc(1, sizeof(ClassName));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void dtor() {
		expr = nullptr;
		ini_expr_ptr = nullptr;
		ERROR = 0;
	}

	static void DELETE(ClassName *classname) {
		if (!classname) {
			return;
		}

		classname->dtor();
		free(classname);
	}

//=============================================================================

	double eval(const char *expression) {
		expr = expression;
		ini_expr_ptr = expression;

		double res = get_G();
		if (!ERROR) {
			return OK;
		} else {
			RAISE_ERROR_SYNTAX(ini_expr_ptr, ERRPOS);
			return 0.0;
		}
	}

};

#endif // RECURSIVE_PARSER