/*
Реализуйте свой стек. Решения, использующие std::stack, получат 1 балл. Решения, хранящие стек в массиве, получат 1.5 балла. Решения, использующие указатели, получат 2 балла.

Гарантируется, что количество элементов в стеке ни в какой момент времени не превышает 10000.

Обработайте следующие запросы:

push n: добавить число n в конец стека и вывести «ok»;
pop: удалить из стека последний элемент и вывести его значение, либо вывести «error», если стек был пуст;
back: сообщить значение последнего элемента стека, либо вывести «error», если стек пуст;
size: вывести количество элементов в стеке;
clear: опустошить стек и вывести «ok»;
exit: вывести «bye» и завершить работу.
Входные данные
В каждой строке входных данных задана одна операция над стеком в формате, описанном выше.

Выходные данные
В ответ на каждую операцию выведите соответствующее сообщение.
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

int main () {
    List *l = List_construct();

    char str[10];
    while (1) {
        scanf("%6s", str);
        if (strcmp(str, "push") == 0) {
            long int val;
            scanf("%ld", &val);
            List_push_front(l, val);
            printf("ok\n");
        } else if (strcmp(str, "pop") == 0) {
            Node *n = List_front(l);
            if (!n) {
                printf("error\n");
            } else {
                printf("%ld\n", n->val);
                List_pop_front(l);
            }
        } else if (strcmp(str, "back") == 0) {
            Node *n = List_front(l);
            if (!n) {
                printf("error\n");
            } else {
                printf("%ld\n", n->val);
            }
        } else if (strcmp(str, "size") == 0) {
            printf("%ld\n", (long int)l->size);
        } else if (strcmp(str, "clear") == 0) {
            List_clear(l);
            printf("ok\n");
        } else if (strcmp(str, "exit") == 0) {
            printf("bye\n");
            break;
        }
    }


    return 0;
}

// O(n)
