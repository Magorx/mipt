

class DecisionStatement {
public:
	virtual int state() = 0;
}

class DecisionQuestion : DecisionStatement {
public:
	virtual int get_answer() = 0;
}

class DecisionTree_Node {
	DecisionStatement *statement;
	DecisionTree_Node *node_true;
	DecisionTree_Node *node_false;
public:
	DecisionTree_Node();
	DecisionTree_Node(DecisionQuestion *question_, DecisionTree_Node* node_true_, DecisionTree_Node *node_false_);

	int state_self(); // ?
}