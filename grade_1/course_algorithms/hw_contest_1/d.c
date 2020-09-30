/*
Гоблины Мглистых гор очень любях ходить к своим шаманам. Так как гоблинов много, к шаманам часто образуются очень длинные очереди. А поскольку много гоблинов в одном месте быстро образуют шумную толку, которая мешает шаманам проводить сложные медицинские манипуляции, последние решили установить некоторые правила касательно порядка в очереди.

Обычные гоблины при посещении шаманов должны вставать в конец очереди. Привилегированные же гоблины, знающие особый пароль, встают ровно в ее середину, причем при нечетной длине очереди они встают сразу за центром.

Так как гоблины также широко известны своим непочтительным отношением ко всяческим правилам и законам, шаманы попросили вас написать программу, которая бы отслеживала порядок гоблинов в очереди.

Входные данные
В первой строке входных данный записано число N (1≤N≤105)  − количество запросов. Следующие N строк содержат описание запросов в формате:

+ i  − гоблин с номером i (1≤i≤N) встаёт в конец очереди.
* i  − привилегированный гоблин с номером i встает в середину очереди.
-  − первый гоблин из очереди уходит к шаманам. Гарантируется, что на момент такого запроса очередь не пуста.
Выходные данные
Для каждого запроса типа - программа должна вывести номер гоблина, который должен зайти к шаманам.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//<KCTF>[EVERYDAY] GENERAL.H ==================================================

const int KCTF_DEBUG_LEVEL = 1; ///< Just a mode for debugging

int           DEBUG_NUMBER = 1;   ///< Just an int for debugging
unsigned char DEBUG_LETTER = 'a'; ///< Just a char for debugging

#define DEBUG_NUMBER_PRINT() printf("[deb] %d [deb]\n", DEBUG_NUMBER++);
#define DEBUG_LETTER_PRINT() printf("[deb] %c [deb]\n", DEBUG_LETTER++);

#define DEBUG_NUMBER_INCR() DEBUG_NUMBER++;
#define DEBUG_LETTER_INCR() DEBUG_LETTER++;

#define DEBUG(LEVEL) if (LEVEL <= KCTF_DEBUG_LEVEL)

const int INT_P = 7777777; /// Poison int

///  Return codes
enum RETURN_CODES {
    ERROR_FILE_NOT_FOUND = -10,
    ERROR_BIG_FILE,
    ERROR_MALLOC_FAIL,
    ERROR_NULL_OBJECT,
    ERROR_NO_RET_CODE,
    ERROR_BAD_ARGS,
    NULL_OBJ_OK = 0,
    RET_OK = 0,
};

//=============================================================================
//<KCTF>[NODE] GENERAL.H ======================================================

#define NODE_VAL_TYPE long int

typedef struct Node_t Node;

struct Node_t {
    NODE_VAL_TYPE val;
    Node *next;
    Node *prev;
};

Node *Node_construct(const NODE_VAL_TYPE val) {
    Node *n = calloc(1, sizeof(Node));
    if (n) {
        n->val = val;
        n->next = NULL;
        n->prev = NULL;
    }
    return n;
}

int Node_destruct(Node *n) {
    if (!n) {
        return NULL_OBJ_OK;
    }
    n->val = INT_P;
    n->next = NULL;
    n->prev = NULL;
    free(n);

    return RET_OK;
}

int Node_push_right(Node *main, Node* n) {
    assert(main);
    assert(n);

    Node *tmp = main->next;
    main->next = n;
    if (tmp) {
        tmp->prev = n;
    }
    n->prev = main;
    n->next = tmp;

    return RET_OK;
}

int Node_push_left(Node *main, Node *n) {
    assert(main);
    assert(n);

    Node *tmp = main->prev;
    main->prev = n;
    if (tmp) {
        tmp->next = n;
    }
    n->next = main;
    n->prev = tmp;

    return RET_OK;
}

int Node_pop(Node *n) {
    if (!n) {
        return NULL_OBJ_OK;
    }

    if (n->next) {
        n->next->prev = n->prev;
    }
    if (n->prev) {
        n->prev->next = n->next;
    }

    return Node_destruct(n);
}

//=============================================================================
//<KCTF>[LIST] GENERAL.H ======================================================

#define LIST_VAL_TYPE NODE_VAL_TYPE

typedef struct List_t List;

struct List_t {
    Node *fictive;
    size_t size;
};

List *List_construct() {
    List *l = calloc(1, sizeof(List));
    if (!l) {
        return l;
    }

    l->fictive = Node_construct(INT_P);
    if (!l->fictive) {
        free(l);
        return NULL;
    }
    l->size = 0;

    return l;
}

int List_erase_node(List *l, Node *n) {
    assert(l);
    assert(n);

    if (l->size <= 0) {
        return ERROR_BAD_ARGS;
    }

    --l->size;
    return Node_pop(n);
}

int List_pop_front(List *l) {
    assert(l);
    assert(l->fictive->next);

    l->fictive->next->prev = l->fictive;
    int ret_code = List_erase_node(l, l->fictive->next);
    if (ret_code) {
        return ret_code;
    }
    if (l->fictive->next) {
        l->fictive->next->prev = NULL;
    }

    if (!l->size) {
        l->fictive->prev = NULL;
    }

    return RET_OK;
}

int List_pop_back(List *l) {
    assert(l);
    assert(l->fictive->prev);

    l->fictive->prev->next = l->fictive;
    int ret_code = List_erase_node(l, l->fictive->prev);
    if (ret_code) {
        return ret_code;
    }

    if (l->fictive->prev) {
        l->fictive->prev->next = NULL;
    }

    if (!l->size) {
        l->fictive->next = NULL;
    }

    return RET_OK;

}

int List_clear(List *l) {
    assert(l);

    while (l->size > 0) {
        int ret_code = List_pop_front(l);
        if (ret_code) {
            return ret_code;
        }
    }

    return RET_OK;
}

int List_destruct(List *l) {
    assert(l);

    int ret_code = List_clear(l);
    if (ret_code) {
        return ret_code;
    }

    l->size = INT_P;
    Node_destruct(l->fictive);
    free(l);

    return RET_OK;
}

Node *List_front(List *l) {
    assert(l);
    return l->fictive->next;
}

Node *List_back(List *l) {
    assert(l);
    if (l->size == 0) {
        return NULL;
    } else {
        return l->fictive->prev;
    }
}

int List_push_front(List *l, const LIST_VAL_TYPE val) {
    assert(l);

    Node *n = Node_construct(val);
    if (!n) {
        return ERROR_NULL_OBJECT;
    }

    int ret_code = Node_push_right(l->fictive, n);
    if (ret_code) {
        return ret_code;
    }
    n->prev = NULL;

    if (l->size == 0) {
        l->fictive->prev = n;
    }
    ++l->size;

    return RET_OK;
}

int List_push_back(List *l, const LIST_VAL_TYPE val) {
    assert(l);

    Node *n = Node_construct(val);
    if (!n) {
        return ERROR_NULL_OBJECT;
    }

    int ret_code = Node_push_left(l->fictive, n);
    if (ret_code) {
        return ret_code;
    }
    n->next = NULL;

    if (l->size == 0) {
        l->fictive->next = n;
    }
    ++l->size;


    return RET_OK;
}

//=============================================================================

char get_pair_bracket(const char c) {
    if (c == '(') {
        return ')';
    } else if (c == '[') {
        return ']';
    } else if (c == '{') {
        return '}';
    } else if (c == ')') {
        return '(';
    } else if (c == ']') {
        return '[';
    } else if (c == '}') {
        return '{';
    } else {
        return -1;
    }
}

char is_opening_bracket(const char c) {
    return c == '(' || c == '[' || c == '{';
}

int main() {
    List *left_list = List_construct();
    List *right_list = List_construct();
    int n = 0;
    scanf("%d", &n);
    n *= 2;

    while (n-->0) {
        char c = 0;
        scanf("%c", &c);
        if (c == '-') {
            long int val = List_front(left_list)->val;
            List_pop_front(left_list);
            printf("%d\n", val);
        } else if (c == '+') {
            long int ind = 0;
            scanf("%ld", &ind);
            List_push_back(right_list, ind);
        } else if (c == '*') {
            long int ind = 0;
            scanf("%ld", &ind);
            List_push_front(right_list, ind);
        }

        if (left_list->size < right_list->size) {
            long int val = List_front(right_list)->val;
            List_push_back(left_list, val);
            List_pop_front(right_list);
        }
    }

    List_destruct(left_list);
    List_destruct(right_list);

    return 0;
}

// O(n)
