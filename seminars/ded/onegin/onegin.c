#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAXSTRLEN = 99;
const int MAXSTRS   = 10000;

const int STROFA_SIZE = 14;
const int RYTHM_DEPTH = 4;

enum ERRORS {
    ERROR_FILE_NOT_FOUND = -10,
    ERROR_BIG_FILE
};

struct Line {
    unsigned char *string;
    int len;
    unsigned char ending[5];
    int strofa_index;
};

typedef struct Line Line_t;

void qqh_sort(void *arr, int elem_cnt, size_t elem_size, int (*comp)(void *elem1, void *elem2));
int compare_lines(const void **elem1, const void **elem2);
int reverse_compare_lines(const void **elem1, const void **elem2);
int rythm_compare_lines(const void **elem1, const void **elem2);

void free_memory(unsigned char **lines, char *fin_name, char *fout_name);
int read_lines(char *file_name, Line_t **lines);
void print_lines(char *file_names, Line_t **lines, int lines_cnt);

void calculate_eding(Line_t *line);
char rythming_lines(Line_t *first, Line_t *second, int depth);
int gen_strofa(Line_t **lines, int lines_cnt, unsigned int *buffer, int rythm_depth);

int main(const int argc, const char **argv) {
    setlocale(LC_ALL,"Russian");

    Line_t **lines = calloc(MAXSTRS, sizeof(Line_t));
    Line_t **lines_ptr = lines;
    for (int i = 0; i < MAXSTRS; ++i) {
        *lines_ptr = calloc(1, sizeof(Line_t));
        (*lines_ptr)->string = calloc(MAXSTRLEN, sizeof(char));
        ++lines_ptr;
    }

    char *fin_name = calloc(MAXSTRLEN, sizeof(char));
    strcpy(fin_name, "onegin.txt");
    char *fout_name = calloc(MAXSTRLEN, sizeof(char));
    strcpy(fout_name, "oneginized.txt");
    if (argc > 1) {
        fin_name = argv[1];
    }
    if (argc > 2) {
        fout_name = argv[2];
    }

    int lines_cnt = read_lines(fin_name, lines);
    if (lines_cnt < 0) {
        free_memory(lines, fin_name, fout_name);
        return 0;
    }

    qsort(lines, lines_cnt, sizeof(char*), rythm_compare_lines);
    printf("%d lines are sorted!\n", lines_cnt);
    print_lines(fout_name, lines, lines_cnt);

    int buffer[STROFA_SIZE];
    gen_strofa(lines, lines_cnt, buffer, RYTHM_DEPTH);
    for (int i = 0; i < STROFA_SIZE; ++i) {
        printf("%s", lines[buffer[i]]->string);
    }

    free_memory(lines, fin_name, fout_name);

    return 0;

}

char is_russian_letter(unsigned char c) {
    return (c >= (unsigned char) 'À' && c <= (unsigned char) 'ß') || (c >= (unsigned char) 'à' && c <= (unsigned char) 'ÿ');
}

char is_letter(unsigned char c) {
    return isalpha(c) || is_russian_letter(c);
}

void swap_ptrs(void **first, void **second) {
    void *tmp = *second;
    *second = *first;
    *first = tmp;
}

void qqh_sort(void *arr, int elem_cnt, size_t elem_size, int (*comp)(void *elem1, void *elem2)) {
    for (int i = 0; i < elem_cnt; ++i) {
        for (int j = 0; j < elem_cnt - 1; ++j) {
            void *first = arr + j * elem_size;
            void *second = arr + (j + 1) * elem_size;
            if (comp(first, second) > 0) {
                swap_ptrs(first, second);
            }
        }
    }
}

int compare_lines(const void **elem1, const void **elem2) {
    unsigned char* str1 = ((Line_t*)(*elem1))->string;
    unsigned char* str2 = ((Line_t*)(*elem2))->string;

    int i = 0;
    int j = 0;
    while (str1[i] && str2[j]) {
        while (!is_letter(str1[i]) && str1[i]) {
            ++i;
        }
        while (!is_letter(str2[j]) && str2[j]) {
            ++j;
        }

        if (str1[i] != str2[i] || str1[i] * str2[i] == 0) {
            return str1[i] - str2[j];
        }

        ++i;
        ++j;
    }
    return str1[i] - str2[j];
}

int reverse_compare_lines(const void **elem1, const void **elem2) {
    return -compare_lines(elem1, elem2);
}

int rythm_compare_lines(const void **elem1, const void **elem2) {
    unsigned char* str1 = ((Line_t*)(*elem1))->string;
    unsigned char* str2 = ((Line_t*)(*elem2))->string;

    int i = strlen(str1) - 1;
    int j = strlen(str2) - 1;
    while (i >= 0 && j >= 0) {
        while (!is_letter(str1[i]) && str1[i]) {
            --i;
        }
        while (!is_letter(str2[j]) && str2[j]) {
            --j;
        }

        if (str1[i] != str2[i] || i * j == 0) {
            return str1[i] - str2[j];
        }

        --i;
        --j;
    }
    return str1[i] - str2[j];
}

void free_memory(unsigned char **lines, char *fin_name, char *fout_name) {
    unsigned char **lines_ptr = lines;
    for (int i = 0; i < MAXSTRS; ++i) {
        free(*lines_ptr);
        ++lines_ptr;
    }
    free(lines);
    free(fin_name);
    free(fout_name);
}

int read_lines(char *file_name, Line_t **lines) {
    FILE *fin = fopen(file_name, "r");
    if (!fin) {
        printf("File '%s' not found!\n", file_name);
        return ERROR_FILE_NOT_FOUND;
    }

    Line_t **lines_ptr = lines;
    Line_t **itterable_ptr = lines_ptr;
    Line_t *line_ptr = *itterable_ptr;
    char *string_ptr = line_ptr->string;
    int lines_cnt = 0;
    while (fgets(string_ptr, sizeof(char) * MAXSTRLEN, fin)) {
        lines_cnt += 1;
        if (lines_cnt == MAXSTRS - 1) {
            printf("Can't handle such a big file!\n");
            return ERROR_BIG_FILE;
        }

        line_ptr->len = strlen(string_ptr);
        calculate_ending(line_ptr);
        line_ptr->strofa_index = lines_cnt % STROFA_SIZE;

        ++itterable_ptr;
        line_ptr = *itterable_ptr;
        string_ptr = line_ptr->string;
    }

    return lines_cnt;
}

void print_lines(char *file_name, Line_t **lines, int lines_cnt) {
    FILE *fout = fopen(file_name, "w");
    for (int i = 0; i < lines_cnt; ++i) {
        fprintf(fout, "%s", lines[i]->string);
    }

    fclose(fout);
}

void calculate_ending(Line_t *line) {
    int i = line->len - 1;
    while(!is_letter(line->string[i]) && i) {
        --i;
    }
    if (!i) {
        return;
    }

    for (int j = 0; j < RYTHM_DEPTH; ++j) {
        line->ending[j] = line->string[i-RYTHM_DEPTH+j+1];
    }
}

char rythming_lines(Line_t *first, Line_t *second, int depth) {
    if (first == second) {
        return 0;
    }

    char *str1 = first->string;
    char *str2 = second->string;

    if (strcmp(first->ending+1, second->ending+1)) {
        return 0;
    }
    return 1;
}

int gen_strofa(Line_t **lines, int lines_cnt, unsigned int *buffer, int rythm_depth) {
    srand(time(NULL));
    int lens[STROFA_SIZE];
    for (int i = 0; i < STROFA_SIZE; ++i) {
        int itter = 0;
        while (1) {
            ++itter;
            if (itter == 100) {
                i = 0;
                continue;
            }
            int line_index = ((((int) rand()) % (lines_cnt / 14)) * 14 + i) % lines_cnt;
            Line_t *line = lines[line_index];
            int len = line->len;
            if (i == 0 || i == 1 || i == 4 || i == 6 || i == 8 || i == 9 || i == 12) {
                buffer[i] = line_index;
                lens[i] = len;
                break;
            } else if (i == 2 || i == 3) {
                if (rythming_lines(line, lines[buffer[i - 2]], rythm_depth)) {
                    buffer[i] = line_index;
                    lens[i] = len;
                    break;
                }
            } else if (i == 5 || i == 7 || i == 10 || i == 13) {
                if (rythming_lines(line, lines[buffer[i - 1]], rythm_depth)) {
                    buffer[i] = line_index;
                    lens[i] = len;
                    break;
                }
            } else {
                if (rythming_lines(line, lines[buffer[i - 3]], rythm_depth)) {
                    buffer[i] = line_index;
                    lens[i] = len;
                    break;
                }
            }
        }
    }
    return 0;
}
