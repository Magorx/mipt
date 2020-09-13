#ifndef ONEGIN_H
#define ONEGIN_H

#include <assert.h>
#include "general.h"

/**
    @brief Calculates an ending of a line

    Ignores all symbols that are not letters in the end of string takes last RHYME_DEPTH chars
    Stores output into given line. Works for rhyming purposes

    @param[in] line line to be process
*/
void calculate_ending(Line_t *line, const size_t rhyme_depth);

/**
    @brief End-starting comparator for two lines

    Compares two lines representing them as reversed without manually doing this

    @param[in] elem1,elem2 lines to compare
    @return an int <0 if elem1<elem2, 0 if elem1=elem2, >0 if elem1>elem2
*/
int rhyme_compare_lines(const void **elem1, const void **elem2);

/**
    @brief Checks if lines are rhymic

    Compares endings of the lines that are calculated by calculate_ending function
    rhyming strings are those having RHYME_DEPTH-1 symbols same in the end and difference in RHYME_DEPTH symbol from the end

    @param[in] first,second given string to check rhyming
    @return true if lines are rhymic, else false
*/
char rhymeing_lines(const Line_t *first, const Line_t *second, const size_t depth);

/**
    @brief Generates new strofa from random strings

    Given NOT MESSED UP lines of Onegin generates indexes of strings to be used in new strofa

    @param[in] lines stringview over text of Onegin
    @param[in] lines_cnt number of lines in text
    @param[out] buffer array of size_t to store output into
    @param[in] RHYME_DEPTH depth of rhyme to check (look at rhymeing_lines for desription)
*/
void gen_strofa(const Line_t **lines, const size_t lines_cnt, unsigned int *buffer, const size_t rhyme_depth);

//=============================================================================
//========================== REALISATON =======================================
//=============================================================================

void calculate_ending(Line_t *line, const size_t rhyme_depth) {
    int i = line->len - 1;
    while(!is_letter(line->string[i]) && i) {
        --i;
    }
    if (!i) {
        return;
    }

    for (int j = 0; j < rhyme_depth; ++j) {
        line->ending[j] = line->string[i - rhyme_depth + j + 1];
    }
}

int rhyme_compare_lines(const void **elem1, const void **elem2) {
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

char rhymeing_lines(const Line_t *first, const Line_t *second, const size_t depth) {
    if (first == second) {
        return 0;
    }

    if (strcmp((char*) first->ending + 1, (char*) second->ending + 1) || first->ending[0] == second->ending[0]) {
        return 0;
    }
    return 1;
}

void gen_strofa(const Line_t **lines, const size_t lines_cnt, unsigned int *buffer, const size_t rhyme_depth) {
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
                if (rhymeing_lines(line, lines[buffer[i - 1]], rhyme_depth)) {
                    buffer[i] = line_index;
                    break;
                }
            } else if (i == 2 || i == 3) {
                if (rhymeing_lines(line, lines[buffer[i - 2]], rhyme_depth)) {
                    buffer[i] = line_index;
                    break;
                }
            } else {
                if (rhymeing_lines(line, lines[buffer[i - 3]], rhyme_depth)) {
                    buffer[i] = line_index;
                    break;
                }
            }
        }
    }
}

#endif // ONEGIN_H
