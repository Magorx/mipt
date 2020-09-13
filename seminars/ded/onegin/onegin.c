#include <assert.h>
#include <ctype.h>
#include<fcntl.h>
#include <io.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <time.h>

#include "general.h"

const int MAXSTRLEN = 99;
const int MAXSTRS   = 10000;

/**
    @brief Sorts array on-place

    Quadratic sort for any type of array with required defined comparator

    @param[in]
    @return Result of |number| - eps < 0 statement
*/

int rythm_compare_lines(const void **elem1, const void **elem2);

void calculate_ending(Line_t *line);
char rythming_lines(Line_t *first, Line_t *second, int depth);
void gen_strofa(Line_t **lines, size_t lines_cnt, unsigned int *buffer, int rythm_depth);

void print_error(int error);

int main(const int argc, char **argv) {
    setlocale(LC_ALL,"Russian");


    char *fin_name  = "onegin.txt";
    char *fout_name = "oneginized.txt";
    if (argc > 1) {
        fin_name = argv[1];
    }
    if (argc > 2) {
        fout_name = argv[2];
    }

    File_t fin;
    int ret = read_file(&fin, fin_name);
    if (ret < 0) {
        print_error(ret);
        free_memory_file(&fin);
    }
    if (fin.lines_cnt < 0) {
        print_error(fin.lines_cnt);
        free_memory_file(&fin);
        return 0;
    }
    for (int i = 0; i < fin.lines_cnt; ++i) {
        calculate_ending(fin.lines[i]);
    }

    qsort(fin.lines, fin.lines_cnt, sizeof(Line_t*), compare_lines_letters);
    printf("%d lines are read!\n", fin.lines_cnt);
    print_file(&fin, fout_name);

    unsigned int buffer[STROFA_SIZE];
    gen_strofa(fin.lines, fin.lines_cnt, buffer, RYTHM_DEPTH);
    for (int i = 0; i < STROFA_SIZE; ++i) {
        printf("%s\n", fin.lines[buffer[i]]->string);
    }

    free_memory_file(&fin);

    return 0;

}

int rythm_compare_lines(const void **elem1, const void **elem2) {
    const Line_t *line1 = *elem1;
    const Line_t *line2 = *elem2;
    const unsigned char* str1 = line1->string;
    const unsigned char* str2 = line2->string;

    int i = line1->len - 1;
    int j = line2->len - 1;
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

void calculate_ending(Line_t *line) {
    int i = line->len - 1;
    while(!is_letter(line->string[i]) && i) {
        --i;
    }
    if (!i) {
        return;
    }

    for (int j = 0; j < RYTHM_DEPTH; ++j) {
        line->ending[j] = line->string[i - RYTHM_DEPTH + j + 1];
    }
}

char rythming_lines(Line_t *first, Line_t *second, int depth) {
    if (first == second) {
        return 0;
    }

    if (strcmp((char*) first->ending + 1, (char*) second->ending + 1) || first->ending[0] == second->ending[0]) {
        return 0;
    }
    return 1;
}

void gen_strofa(Line_t **lines, size_t lines_cnt, unsigned int *buffer, int rythm_depth) {
    assert(lines);
    assert(buffer);

    srand(time(NULL));
    for (int i = 0; i < STROFA_SIZE; ++i) {
        int itter = 0;
        while (1) {
            ++itter;
            if (itter == 100) {
                i = 0;
                continue;
            }

            int line_index = (((int) rand()) % (lines_cnt / 14)) * 14 + i;
            Line_t *line = lines[line_index];
            if (i == 0 || i == 1 || i == 4 || i == 6 || i == 8 || i == 9 || i == 12) {  // COMMENTS
                buffer[i] = line_index;
                break;
            } else if (i == 5 || i == 7 || i == 10 || i == 13) {
                if (rythming_lines(line, lines[buffer[i - 1]], rythm_depth)) {
                    buffer[i] = line_index;
                    break;
                }
            } else if (i == 2 || i == 3) {
                if (rythming_lines(line, lines[buffer[i - 2]], rythm_depth)) {
                    buffer[i] = line_index;
                    break;
                }
            } else {
                if (rythming_lines(line, lines[buffer[i - 3]], rythm_depth)) {
                    buffer[i] = line_index;
                    break;
                }
            }
        }
    }
}

void print_error(int error) {
    if (error == ERROR_FILE_NOT_FOUND) {
        printf("[ERR] File not found!\n");
    } else if (error == ERROR_BIG_FILE) {
        printf("[ERR] Can't handle such a big file!\n");
    } else if (error == ERROR_MALLOC_FAIL) {
        printf("[ERR] Can't allocate memory\n");
    } else {
        printf("[ERR](~!~)WERROREHUTGEERRORF(~!~)[ERR]\n");
    }
}

