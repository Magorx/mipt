/*
Гистограмма является многоугольником, сформированным из последовательности прямоугольников, выровненных на общей базовой линии. Прямоугольники имеют равную ширину, но могут иметь различные высоты. Обычно гистограммы используются для представления дискретных распределений, например, частоты символов в текстах. Отметьте, что порядок прямоугольников очень важен. Вычислите область самого большого прямоугольника в гистограмме, который также находится на общей базовой линии.

Входные данные
В первой строке входного файла записано число N (0<N≤106)  − количество прямоугольников гистограммы. Затем следует N целых чисел h1…hn, где 0≤hi≤109. Эти числа обозначают высоты прямоугольников гистограммы слева направо. Ширина каждого прямоугольника равна 1.

Выходные данные
Выведите площадь самого большого прямоугольника в гистограмме. Помните, что этот прямоугольник должен быть на общей базовой линии.
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

#define NODE_VAL_TYPE long long int

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

long long int max(long long int a, long long int b) {
    if (a > b) {
        return a;
    }
    return b;
}

//=============================================================================

int main () {
    List *l = List_construct();
    List *l_id = List_construct();

    int n = 0;
    scanf("%d", &n);
    long long int ans = 0;
    for (long long int i = 0; i <= n; ++i) {
        long long int x = 0;
        if (i < n) {
            scanf("%lld", &x);
        } else {
            x = 0;
        }
        if (l->size == 0) {
            List_push_front(l, x);
            List_push_front(l_id, i);
            continue;
        }

        char to_push = 1;
        while (l->size) {
            long long int top = List_front(l)->val;
            if (top < x) {
                List_push_front(l, x);
                List_push_front(l_id, i);
                break;
            } else if (top > x) {
                long long int id = List_front(l_id)->val;
                ans = max(ans, (i - id) * top);
                if (l->size > 1) {
                    if (l->fictive->next->next->val >= x) {
                        List_pop_front(l);
                        List_pop_front(l_id);
                    } else {
                        l->fictive->next->val = x;
                        to_push = 0;
                        break;
                    }
                } else {
                    l->fictive->next->val = x;
                    to_push = 0;
                    break;
                }
            } else {
                to_push = 0;
                break;
            }
        }
        if (to_push) {
            List_push_front(l, x);
            List_push_front(l_id, i);
        }
    }

    printf("%lld\n", ans);

    List_destruct(l);
    List_destruct(l_id);

    return 0;
}

// O(n)
