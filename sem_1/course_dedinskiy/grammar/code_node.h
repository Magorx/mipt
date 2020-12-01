#ifndef CODENODE_H
#define CODENODE_H

#include <cstdio>

#include "general/cpp/stringview.hpp"

//=============================================================================
// CodeNode ===================================================================

enum CODE_NODE_TYPE {
	NONE        = 0,
	OPERATION   = 1,
	VARIABLE    = 2,
	VALUE       = 3,
	ID          = 4,
};

union CodeNodeData {
	int op;
	int var;
	double val;
	StringView *id;
};

class CodeNode {
private:
// data =======================================================================
char type;
CodeNodeData data;

CodeNode *L;
CodeNode *R;
//=============================================================================


public:
	CodeNode            (const CodeNode&) = delete;
	CodeNode &operator= (const CodeNode&) = delete;

	CodeNode():
	type(0),
	data(),
	L(nullptr),
	R(nullptr)
	{}

	~CodeNode() {}

	void ctor() {
		type = NONE;
		data.val = 0;
		L = nullptr;
		R = nullptr;
	}

	static CodeNode *NEW() {
		CodeNode *cake = (CodeNode*) calloc(1, sizeof(CodeNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void ctor(const char type_, const int op_or_var, CodeNode *L_=nullptr, CodeNode *R_=nullptr) {
		type = type_;
		if (type == OPERATION) {
			data.op = op_or_var;
		} else {
			data.var = op_or_var;
		}
		L = L_;
		R = R_;
	}

	static CodeNode *NEW(const char type_, const int op_or_var, CodeNode *L_=nullptr, CodeNode *R_=nullptr) {
		CodeNode *cake = (CodeNode*) calloc(1, sizeof(CodeNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor(type_, op_or_var, L_, R_);
		return cake;
	}

	void ctor(const char type_, const double val_, CodeNode *L_=nullptr, CodeNode *R_=nullptr) {
		type = type_;
		data.val = val_;
		L = L_;
		R = R_;
	}

	static CodeNode *NEW(const char type_, const double val_, CodeNode *L_=nullptr, CodeNode *R_=nullptr) {
		CodeNode *cake = (CodeNode*) calloc(1, sizeof(CodeNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor(type_, val_, L_, R_);
		return cake;
	}

	void ctor(const char type_, StringView *id_, CodeNode *L_=nullptr, CodeNode *R_=nullptr) {
		type = type_;
		data.id = id_;
		L = L_;
		R = R_;
	}

	static CodeNode *NEW(const char type_, StringView *id_, CodeNode *L_=nullptr, CodeNode *R_=nullptr) {
		CodeNode *cake = (CodeNode*) calloc(1, sizeof(CodeNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor(type_, id_, L_, R_);
		return cake;
	}

	void dtor() {
		type = NONE;
		data.val = 0;
		L = nullptr;
		R = nullptr;
	}

	static void DELETE(CodeNode *node, bool recursive = false) {
		if (!node) {
			return;
		}

		if (recursive) {
			if (node->L) DELETE(node->L, recursive);
			if (node->R) DELETE(node->R, recursive);
		}

		node->dtor();
		free(node);
	}

//=============================================================================

	void set_L(CodeNode *L_) {
		L = L_;
	}

	void set_R(CodeNode *R_) {
		R = R_;
	}

	void set_LR(CodeNode *L_, CodeNode *R_) {
		L = L_;
		R = R_;
	}

	void set_type(const char type_) {
		type = type_;
	}

	void set_op(const int op_) {
		set_type(OPERATION);
		data.op = op_;
	}

	void set_var(const int var_) {
		set_type(VARIABLE);
		data.var = var_;
	}

	void set_val(const double val_) {
		set_type(VALUE);
		data.val = val_;
	}

	void set_id(StringView *id_) {
		set_type(ID);
		data.id = id_;
	}

	char get_type() {
		return type;
	}

	int get_op() {
		return data.op;
	}

	int get_var() {
		return data.var;
	}

	double get_val() {
		return data.val;
	}

	StringView *get_id() {
		return data.id;
	}

	bool is_op() {
		return type == OPERATION;
	}

	bool is_var() {
		return type == VARIABLE;
	}

	bool is_val() {
		return type == VALUE;
	}

	bool is_id() {
		return type == ID;
	}
	
// ============================================================================

	void space_dump(FILE *file = stdout) {
		if (L) {
			fprintf(file, "(");
			L->space_dump();
			fprintf(file, ")");
		}

		//fprintf(file, "(");
		if (is_op()) {
			fputc(data.op, file);
		} else if (is_var()) {
			fprintf(file, "{%d}", data.var);
		} else if (is_val()) {
			fprintf(file, "%lg", data.val);
		} else if (is_id()) {
			data.id->print();
		} else {
			fprintf(file, "ERR");
		}
		//fprintf(file, ")");

		if (R) {
			fprintf(file, "(");
			R->space_dump();
			fprintf(file, ")");
		}
	}

};

#endif // CODENODE_H