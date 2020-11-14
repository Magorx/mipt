#include "decision_tree.h"

#include <cstdio>

//=============================================================================
// Statement and Question =====================================================

DecisionStatement::DecisionStatement(const String &statement_) {
	statement = statement_;
}

int DecisionStatement::state(char *end) {
	statement.print();
	printf("%s", end);
	return 0;
}

int DecisionQuestion::state() {
	printf("Is it true that your object ");
	DecisionStatement::state((char*) "?\n");

	return 0;
}

//=============================================================================
// Node =======================================================================

DecisionTreeNode::DecisionTreeNode() {
	statement  = nullptr;
	node_true  = nullptr;
	node_false = nullptr;
}

DecisionTreeNode::DecisionTreeNode(AbstractDecisionStatement *statement_, DecisionTreeNode* node_true_, DecisionTreeNode *node_false_) {
	statement  = statement_ ;
	node_true  = node_true_ ;
	node_false = node_false_;
}

void DecisionTreeNode::set_true(DecisionTreeNode* node) {
	node_true = node;
}

void DecisionTreeNode::set_false(DecisionTreeNode* node) {
	node_false = node;
}

DecisionTreeNode *DecisionTreeNode::proceed(const int answer) {
	if (answer) {
		return node_true;
	} else {
		return node_false;
	}
}

int DecisionTreeNode::state() {
	return statement->state();
}

//=============================================================================
// Tree =======================================================================

DecisionTree::DecisionTree() {
	root = nullptr;
}

DecisionTreeNode *DecisionTree::read_node(File *file) {
	const unsigned char *c = file->cc;
	Char_get_next_symb(&c);
	if (*c != SYMB_QUOTE) {
		printf("Invalid input file\n");
		return nullptr;
	}

	String node_statement;
	c += node_statement.read(c);

	Char_get_next_symb(&c);
	file->cc = c;

	if (*c == SYMB_OPEN_NODE) {
		DecisionQuestion question(node_statement);
		DecisionTreeNode *node = new DecisionTreeNode(&question);
		node->set_true (read_node(file));
		node->set_false(read_node(file));

		return node;
	} else if (*c == SYMB_CLOSE_NODE) {
		DecisionStatement statement(node_statement);
		DecisionTreeNode *node = new DecisionTreeNode(&statement);

		++file->cc;

		return node;
	} else {
		printf("Invalid input file\n");
		return nullptr;
	}
}

int DecisionTree::load(const char *file_name) {
	File file = {};
	if (File_construct(&file, file_name) < 0) {
		printf("File (%s) doesn't exist\n", file_name);
	}

	root = read_node(&file);
	File_destruct(&file);
}

int DecisionTree::save(const char *file_name) {

}
