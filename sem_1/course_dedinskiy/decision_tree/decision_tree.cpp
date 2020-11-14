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

void DecisionStatement::dump(FILE *file_ptr) const {
	statement->print(file_ptr);
}

int answer_is_yes(char *answer) {
	return answer[0] == 'y' || answer[0] == 'Y' || answer[0] == '+' || answer[0] == '1';
}

int get_and_process_answer() {
	char answer[51];
	scanf("%50s", answer);

	if (answer_is_yes(answer)) {
		return YES;
	} else {
		return NO;
	}
}

int DecisionDefinition::state(char *end) {
	printf("Yout object is ");
	DecisionStatement::state(end);
	printf("!\n");
	printf("Am I right?\n");

	return GUESS + get_and_process_answer();
}

int DecisionQuestion::state(char *end) {
	printf("Is it true that your object ");
	DecisionStatement::state(end);
	printf("?\n");

	return QUESTION + get_and_process_answer();
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

DecisionTreeNode* DecisionTreeNode::get_node_true() const {
	return node_true;
}

DecisionTreeNode* DecisionTreeNode::get_node_false() const {
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

void DecisionTreeNode::dump(FILE *file_ptr) const {
	statement->dump(file_ptr);
}

int DecisionTreeNode::statement_length() {
	return statement->size();
}

//=============================================================================
// Tree =======================================================================

DecisionTree::DecisionTree() {
	root = nullptr;
}

DecisionTreeNode *DecisionTree::load_node(File *file) {
	const unsigned char *c = file->cc;
	Char_get_next_symb(&c);
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
	c += node_statement->read(c, false, '"');

	++c;
	Char_get_next_symb(&c);
	file->cc = c;

	if (*c == SYMB_OPEN_NODE) {
		DecisionDefinition *question = new DecisionDefinition(node_statement);
		DecisionTreeNode *node = new DecisionTreeNode(question);

		++file->cc;

		node->set_true (load_node(file));
		node->set_false(load_node(file));

		Char_get_next_symb(&file->cc);
		++file->cc;

		return node;
	} else if (*c == SYMB_CLOSE_NODE || *c == SYMB_QUOTE) {
		DecisionStatement *statement = new DecisionStatement(node_statement);
		DecisionTreeNode *node = new DecisionTreeNode(statement);

		return node;
	} else {
		printf("Invalid input file\n");
		return nullptr;
	}
}

int DecisionTree::load(const char *file_name) {
	if (file_name == nullptr) {
		printf("nullptr file_name, DB is NOT read\n");
		return -1;
	}

	File file = {};
	if (File_construct(&file, file_name) < 0) {
		printf("File (%s) doesn't exist\n", file_name);
	}

	root = load_node(&file);
	File_destruct(&file);

	return 0;
}

void file_printf_tab(FILE *file_ptr, const int tab) {
	for (int i = 0; i < tab; ++i) {
		fprintf(file_ptr, "    ");
	}
}

int DecisionTree::save_node(const DecisionTreeNode* node, int depth, bool is_true_node, FILE *file_ptr) {
	if (!node) {
		printf("Invalid tree: a node is not presented\n");
		return -1;
	}

	file_printf_tab(file_ptr, depth);
	fprintf(file_ptr, "%c", SYMB_QUOTE);
	node->dump(file_ptr);
	fprintf(file_ptr, "%c", SYMB_QUOTE);

	fprintf(file_ptr, "\n");
	if (node->get_node_true()) {
		file_printf_tab(file_ptr, depth);
		fprintf(file_ptr, "%c\n", SYMB_OPEN_NODE);

		save_node(node->get_node_true(),  depth + 1, true,  file_ptr);
		save_node(node->get_node_false(), depth + 1, false, file_ptr);

		file_printf_tab(file_ptr, depth);
		fprintf(file_ptr, "%c\n", SYMB_CLOSE_NODE);
	}

	if (is_true_node) {
		fprintf(file_ptr, "\n");
	}

	return 0;
}

int DecisionTree::save(const char *file_name) {
	if (file_name == nullptr) {
		printf("nullptr file_name, DB is NOT saved\n");
		return -1;
	}

	FILE *file = fopen(file_name, "w");
	if (!file) {
		printf("file was not opened, DB is NOT saved\n");
		return -1;
	}

	save_node(root, 0, false, file);

	return 0;
}

void DecisionTree::dump(DecisionTreeNode *node, int depth, int to_format_cnt, int maxlen, FILE *file_ptr) const {
	if (!node) {return;}

	dump(node->get_node_true(), depth + 1, to_format_cnt + 1, maxlen, file_ptr);

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

	node->dump(file_ptr);
	for (int i = 0; i < maxlen - node->statement_length() - 1; ++i) {
		printf("-");
	}
	printf("->|\n");
	dump(node->get_node_false(), depth + 1, to_format_cnt + 1, maxlen, file_ptr);
}

void DecisionTree::dump(FILE *file_ptr) const {
	dump(root, 0, 0, MAX_STATEMENT_LEN, file_ptr);
}
