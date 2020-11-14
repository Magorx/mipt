#include "decision_tree.h"

#include <cstdio>

//=============================================================================
// Statement and Question =====================================================

DecisionStatement::DecisionStatement(String *statement_) {
	statement = statement_;
}

int DecisionStatement::state(char *end) {
	statement->print();
	printf("%s", end);
	return 0;
}

int DecisionStatement::size() {
	return (int) statement->length();
}

void DecisionStatement::dump() {
	statement->print();
}

int DecisionQuestion::state(char *end) {
	printf("Is it true that your object ");
	DecisionStatement::state(end);

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

DecisionTreeNode* DecisionTreeNode::get_node_true() {
	return node_true;
}

DecisionTreeNode* DecisionTreeNode::get_node_false() {
	return node_false;
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

void DecisionTreeNode::dump() {
	statement->dump();
}

size_t DecisionTreeNode::statement_length() {
	return statement->size();
}

//=============================================================================
// Tree =======================================================================

DecisionTree::DecisionTree() {
	root = nullptr;
}

DecisionTreeNode *DecisionTree::read_node(File *file) {
	// printf("->\n");
	const unsigned char *c = file->cc;
	if (*c == SYMB_CLOSE_NODE || *c == SYMB_OPEN_NODE) {
		++c;
	}
	Char_get_next_symb(&c);
	// printf("cur_c: [%c]\n", *c);
	if (*c != SYMB_QUOTE) {
		printf("Invalid input file\n");
		return nullptr;
	}

	if (*c != SYMB_QUOTE) {
		printf("Invalid input file\n");
		return nullptr;
	}

	++c;
	String *node_statement = new String();
	c += node_statement->read(c);
	(*node_statement)[node_statement->length() - 1] = '\0';
	// printf("read: |");
	// node_statement->print();
	// printf("|\n");

	Char_get_next_symb(&c);
	file->cc = c;
	// printf("cur_c_now: [%c]\n", *c);

	if (*c == SYMB_OPEN_NODE) {
		// printf("question!\n");
		DecisionQuestion *question = new DecisionQuestion(node_statement);
		DecisionTreeNode *node = new DecisionTreeNode(question);

		node->set_true (read_node(file));
		node->set_false(read_node(file));

		return node;
	} else if (*c == SYMB_CLOSE_NODE || *c == SYMB_QUOTE) {
		// printf("statement!\n");
		DecisionStatement *statement = new DecisionStatement(node_statement);
		DecisionTreeNode *node = new DecisionTreeNode(statement);

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

	return 0;
}

int DecisionTree::save(const char *file_name) {
	printf("%s\n", file_name);
	return 0;
}

void DecisionTree::dump(DecisionTreeNode *node, int depth, int to_format_cnt, int maxlen) {
	if (!node) {return;}

	dump(node->get_node_true(), depth + 1, to_format_cnt + 1, maxlen);

	for (int i = 0; i < depth; ++i) {
		for (int j = 0; j < maxlen; ++j) {
			printf(" ");
		}
		if (depth - to_format_cnt- 1 <= i) {
			printf("|");
		} else {
			printf(" ");
		}
	}

	node->dump();
	for (size_t i = 0; i < maxlen - node->statement_length(); ++i) {
		printf(" ");
	}
	printf(" |\n");
	dump(node->get_node_false(), depth + 1, to_format_cnt + 1, maxlen);
}

void DecisionTree::dump() {
	dump(root, 0, 0, 10);
}
