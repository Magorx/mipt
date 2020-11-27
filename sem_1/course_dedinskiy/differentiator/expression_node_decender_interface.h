#ifndef EXPRESSION_NODE_DECENDER_INTERFACE
#define EXPRESSION_NODE_DECENDER_INTERFACE

class ExprNode;

class ExprNodeDecender {
private:
// data =======================================================================
	ExprNode *op_node;
	ExprNode *elem_node;
//=============================================================================

public:
	ExprNodeDecender ();
	~ExprNodeDecender();

	void ctor();
	void ctor(ExprNode *op_node_);

	static ExprNodeDecender *NEW();
	static ExprNodeDecender *NEW(ExprNode *op_node_);

	void dtor();

	static void DELETE(ExprNodeDecender *classname);

//=============================================================================

	bool is_end();

	bool decend();
	bool next();

	void set_operand(ExprNode *new_elem);

	ExprNode *get_op_node();
	ExprNode *get_elem_node();

};

#endif // EXPRESSION_NODE_DECENDER_INTERFACE