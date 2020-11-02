#include <stdlib.h>

#include "general.h"

#ifndef LIST_TYPE
#error LIST_TYPE must be defined to work with KCTF_List
#endif

struct Node_t;
typedef struct Node_t {
	int prev;
	LIST_TYPE data;
	int next;
} Node;

typedef struct List_t {
	Node *buffer;
	int fictive_node;

	size_t capacity;
	size_t size;
	int head;
	int tail;
	int free_head;

	int max_sorted_index;

	int graphviz_dumper_cnt;
} List;

enum LIST_ERROR_CODES {
	ERROR_BROCKEN_LINKS = -7777,
	ERROR_UNEXPECTED_LOOP,
	ERROR_NODE_NOT_IN_LIST,
};

List *new_List();
int   delete_List();

int List_push_next(List *cake, Node *node, LIST_TYPE data);
int List_push_prev();

int List_push_front();
int List_push_back();

//=============================================================================
// Implementation =============================================================

int List_head(const List *cake) {
	return cake->buffer[cake->fictive_node].next;
}

int List_tail(const List *cake) {
	return cake->buffer[cake->fictive_node].prev;
}

int List_valid(const List *cake) {
	return OK;
	if (!cake) {
		RETURNING_VERIFY(ERROR_NULL_OBJECT);
	}

	if (!cake->buffer) {
		RETURNING_VERIFY(ERROR_NULL_BUFFER);
	}

	{
		char *visited = (char*) calloc(cake->capacity, sizeof(char));
		int head = List_head(cake);
		int tail = List_tail(cake);
		for (int i = head; (int) i != tail; i = cake->buffer[i].next) {
			if (visited[i]) {
				RETURNING_VERIFY(ERROR_UNEXPECTED_LOOP);
			}
			visited[i] = 1;
		}
		visited[tail] = 1;
		int visited_sum = 0;
		for (size_t i = 0; i < cake->capacity; ++i) {
			visited_sum += (int) visited[i];
		}
		if (visited_sum != (int) cake->size && cake->size != 0) {
			RETURNING_VERIFY(ERROR_BROCKEN_LINKS);
		}
		free(visited);
	}

	return 0;
}

List *new_List() {
	size_t cap = STANDART_INIT_SIZE;
	
	List *l = (List*) calloc(1, sizeof(List));
	if (!l) {
		return NULL;
	}

	l->buffer = (Node*) calloc(cap, sizeof(Node));
	if (!l->buffer) {
		free(l);
		return NULL;
	}

	l->capacity = cap;
	l->size = 0;
	l->fictive_node = 0;
	l->free_head = 1;

	for (size_t i = 0; i < cap; ++i) {
		l->buffer[i].next = (int) i + 1;
		l->buffer[i].prev = 0;
	}
	l->buffer[0].next = 0;

	l->max_sorted_index = -1;
	l->graphviz_dumper_cnt = 0;

	VERIFY_T(List_valid(l) == OK, List*);
	return l;
}

int delete_List(List *cake) {
	VERIFY_OK(List_valid(cake));

;	free(cake->buffer);
	
	cake->capacity  = (size_t) KCTF_POISON;

	free(cake);

	return 0;
}

int List_dump(const List *cake) {
	VERIFY_OK(List_valid(cake));

	int head = List_head(cake);
	int tail = List_tail(cake);
	for (int i = head; i != tail; i = cake->buffer[i].next) {
		printf("[%d](%d) -> ", i, cake->buffer[i].data);
	}
	printf("[%d](%d)", tail, cake->buffer[tail].data);
	printf("\n");

	return 0;
}

int List_set_capacity(List *cake, const size_t capacity) {
	VERIFY_OK(List_valid(cake));

	if (cake->size >= capacity - 1) {
		RETURNING_VERIFY(ERROR_REALLOC_FAIL);
	}

	Node *new_ptr = realloc(cake->buffer, capacity * sizeof(Node));
	if (!new_ptr) {
		RETURNING_VERIFY(ERROR_REALLOC_FAIL);
	}

	// printf("%d -> %d\n", cake->buffer, new_ptr);

	cake->buffer = new_ptr;
	// printf("%d -> %d\n", cake->buffer, new_ptr);
	for (size_t i = cake->capacity; i < capacity; ++i) {
		// printf("%d\n", i);
		cake->buffer[i].next = (int) i + 1;
		cake->buffer[i].prev = 0;
	}
	cake->capacity = capacity;

	return OK;
}

int List_set_node_value(List *cake, const int node, const int left_node, const int right_node, const LIST_TYPE *data) {
	cake->buffer[node].data = *data;
	cake->buffer[node].next =  node;
	cake->buffer[node].prev =  node;

	cake->buffer[left_node].next = node;
	cake->buffer[node].prev = left_node;

	cake->buffer[right_node].prev = node;
	cake->buffer[node].next = right_node;

	return 0;
}

int List_push_right(List *cake, int node, LIST_TYPE data) {
	VERIFY_OK(List_valid(cake));
	if (cake->buffer[node].prev == (int) KCTF_POISON) {
		RETURNING_VERIFY(ERROR_NODE_NOT_IN_LIST);
	}

	int next_free = cake->free_head;
	if ((size_t) next_free >= cake->capacity - 1) {
		VERIFY_OK(List_set_capacity(cake, cake->capacity * 2));
	}

	cake->free_head = cake->buffer[next_free].next;

	int next_node = cake->buffer[node].next;
	List_set_node_value(cake, next_free, node, next_node, &data);
	
	cake->size += 1;
	cake->max_sorted_index = node < cake->max_sorted_index ? node : cake->max_sorted_index;

	return 0;
}

int List_push_left(List *cake, int node, LIST_TYPE data) {
	VERIFY_OK(List_valid(cake));
	if (cake->buffer[node].prev == (int) KCTF_POISON) {
		RETURNING_VERIFY(ERROR_NODE_NOT_IN_LIST);
	}

	int next_free = cake->free_head;
	if ((size_t) next_free >= cake->capacity - 1) {
		VERIFY_OK(List_set_capacity(cake, cake->capacity * 2));
	}

	cake->free_head = cake->buffer[next_free].next;

	int prev_node = cake->buffer[node].prev;
	List_set_node_value(cake, next_free, prev_node, node, &data);
	cake->size += 1;

	cake->max_sorted_index = node - 1 < cake->max_sorted_index ? node - 1 : cake->max_sorted_index;

	return 0;
}

int List_push_front(List *cake, LIST_TYPE data) {
	return List_push_left(cake, cake->fictive_node, data);
}

int List_push_back(List *cake, LIST_TYPE data) {
	return List_push_right(cake, cake->fictive_node, data);
}

int List_pop(List *cake, const int node) {
	VERIFY_OK(List_valid(cake));
	VERIFY(cake->size > 0);

	int next_node = cake->buffer[node].next;
	int prev_node = cake->buffer[node].prev;

	cake->buffer[node].next = prev_node;
	cake->buffer[node].prev = next_node;

	--cake->size;
	cake->max_sorted_index = node - 1 < cake->max_sorted_index ? node - 1 : cake->max_sorted_index;

	return 0;
}

int List_linear_optimization(List *cake) {
	VERIFY_OK(List_valid(cake));

	Node *new_buffer = (Node*) calloc(cake->capacity, sizeof(Node));
	int node_index = cake->fictive_node;
	int inted_size = (int) cake->size;
	for (int i = 0; i <= inted_size; ++i) {
		new_buffer[i].data = cake->buffer[node_index].data;
		new_buffer[i].next = i + 1;
		new_buffer[i].prev = i - 1;
		node_index = cake->buffer[node_index].next;
	}
	new_buffer[cake->size].next = 0;
	new_buffer[0].prev = (int) cake->size;
	free(cake->buffer);

	cake->buffer = new_buffer;

	cake->max_sorted_index = inted_size;

	return 0;
}

int List_linear_index_search(const List *cake, int index) {
	VERIFY_OK(List_valid(cake));
	++index;
	if (index < 1) {
		RETURNING_VERIFY(ERROR_BAD_ARGS);
	}

	printf("%d vs %d\n", index, cake->max_sorted_index);
	if (index <= cake->max_sorted_index) {
		return index;
	} else {
		int node = List_head(cake);
		for (int i = 1; i < index; ++i) {
			node = cake->buffer[node].next;
		}
		return node;
	}
}

//=============================================================================

int List_graphviz_dump_node(List *cake, FILE *fout, char *node_format, const int node) {
	int next = cake->buffer[node].next;
	int prev = cake->buffer[node].prev;

	if (node == cake->fictive_node) {
		fprintf(fout, "node%d[shape=diamond, color=black, label=\"Fictive\"];", node);
		fprintf(fout, "node%d->node%d:index;\n", node, next);
		fprintf(fout, "node%d:next->node%d;\n", prev, node);
		return 0;
	} else {
		fprintf(fout, node_format, node, node, cake->buffer[node].next, cake->buffer[node].data, cake->buffer[node].prev);
		fprintf(fout, "\n");
	}

	if (next != cake->fictive_node) {
		fprintf(fout, "node%d:index->node%d:index [constraint=false, color=crimson];\n", node, next);
	} else {
		fprintf(fout, "node%d:index->node%d;", node, next);
	}

	if (prev != cake->fictive_node) {
		fprintf(fout, "node%d:next->node%d:prev [constraint=true, color=dodgerblue2];\n", node, prev);
	} else {
		fprintf(fout, "node%d:next->node%d;", node, prev);
	}

	return 0;
}

int List_graphviz_new_output_name(List *cake, const char *output_file_name, char *output_name) {
	size_t of_len = strlen(output_file_name);
	char *output_name_format = (char*) calloc(of_len + 20, sizeof(char));
	
	strcpy(output_name_format, output_file_name);
	output_name_format[of_len + 0] = '%';
	output_name_format[of_len + 1] = 'd';
	sprintf(output_name, output_name_format, cake->graphviz_dumper_cnt++);
	
	free(output_name_format);

	return 0;
}

int List_graphviz_dump(List *cake, const char *output_file_name) {
	VERIFY_OK(List_valid(cake));
	const char *tmp_graphviz_file_name = "gv_dump.dt";

	size_t of_len = strlen(output_file_name);
	char *output_name = (char*) calloc(of_len + 20, sizeof(char));
	List_graphviz_new_output_name(cake, output_file_name, output_name);


	FILE *dot_file = fopen(tmp_graphviz_file_name, "w");
	fprintf(dot_file, "digraph list {rankdir=\"LR\";\n");

	char *node_format = (char*) calloc(1000, sizeof(char));
	
	FILE *node_format_file = fopen("graphviz_node_format.gv", "r");
	fread(node_format, sizeof(char), 1000, node_format_file);
	fclose(node_format_file);

	//printf("%s\n", node_format);

	int head = List_head(cake);
	int tail = List_tail(cake);
	for (int node = head; ; node = cake->buffer[node].next) {
		List_graphviz_dump_node(cake, dot_file, node_format, node);

		if (node == cake->fictive_node) {
			break;
		}
	}

	fprintf(dot_file, "}\n");
	fclose(dot_file);

	char *generate_picture_command = (char*)calloc(100, sizeof(char));
	sprintf(generate_picture_command, "dot %s -Tsvg -o%s.svg", tmp_graphviz_file_name, output_name);

	char *view_picture_command = (char*)calloc(100, sizeof(char));
	sprintf(view_picture_command, "eog %s.svg", output_name);
	
	system(generate_picture_command);
	//system(view_picture_command);

	free(node_format);
	free(output_name);
	free(generate_picture_command);
	free(view_picture_command);

	return 0;
}

int List_graphviz_generate_html(const List *cake, const char *output_file_name) {
	VERIFY_OK(List_valid(cake));
	const char *tmp_html_file_name = "list_dump.html";

	FILE *html_fout = fopen(tmp_html_file_name, "w");
	fprintf(html_fout, "<pre><tt>\n");

	const size_t of_len = strlen(output_file_name);
	char *cur_gv_file_name_format = (char*) calloc(of_len + 20, sizeof(char));
	strcpy(cur_gv_file_name_format, output_file_name);
	strcat(cur_gv_file_name_format, "%d.svg");
	
	char *cur_gv_file_name = (char*) calloc(of_len + 20, sizeof(char));

	for (int i = 0; i < cake->graphviz_dumper_cnt; ++i) {
		sprintf(cur_gv_file_name, cur_gv_file_name_format, i);
		//printf("|%s| -> |%s|\n", cur_gv_file_name, tmp_html_file_name);
		fprintf(html_fout, "[%d]\n<img src=\"%s\">\n", i, cur_gv_file_name);
	}

	fclose(html_fout);

	free(cur_gv_file_name_format);
	free(cur_gv_file_name);

	system("google-chrome list_dump.html");

	return 0;
}