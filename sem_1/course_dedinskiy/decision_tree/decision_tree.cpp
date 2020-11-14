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

int DecisionQuestion::state(char *end) {
	//printf("Is it true that your object ");
	DecisionStatement::state((char*) "?");

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
	printf("->\n");
	const unsigned char *c = file->cc;
	if (*c == SYMB_CLOSE_NODE || *c == SYMB_OPEN_NODE) {
		++c;
	}
	Char_get_next_symb(&c);
	printf("cur_c: [%c]\n", *c);
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
	printf("read: |");
	node_statement->print();
	printf("|\n");

	Char_get_next_symb(&c);
	file->cc = c;
	printf("cur_c_now: [%c]\n", *c);

	if (*c == SYMB_OPEN_NODE) {
		printf("question!\n");
		DecisionQuestion *question = new DecisionQuestion(node_statement);
		DecisionTreeNode *node = new DecisionTreeNode(question);

		node->set_true (read_node(file));
		node->set_false(read_node(file));

		return node;
	} else if (*c == SYMB_CLOSE_NODE || *c == SYMB_QUOTE) {
		printf("statement!\n");
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
}

int DecisionTree::save(const char *file_name) {

}

void DecisionTree::dump(DecisionTreeNode *node, int depth, int to_format_cnt, int maxlen) {
	if (!node) {return;}

	dump(node->node_true, depth + 1, to_format_cnt + 1, maxlen);

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

	node->state();
	for (int i = 0; i < maxlen - ((DecisionStatement*)(node->statement))->statement->length() - (node->node_true != nullptr); ++i) {
		printf(" ");
	}
	printf(" |\n");
	dump(node->node_false, depth + 1, to_format_cnt + 1, maxlen);
}

void DecisionTree::dump() {
	dump(root, 0, 0, 10);
}
