/*
Вася написал на длинной полоске бумаги большое число и решил похвастаться своему старшему брату Пете этим достижением. Но только он вышел из комнаты, чтобы позвать брата, как его сестра Катя вбежала в комнату и разрезала полоску бумаги на несколько частей. В результате на каждой части оказалось одна или несколько идущих подряд цифр.

Теперь Вася не может вспомнить, какое именно число он написал. Только помнит, что оно было очень большое. Чтобы утешить младшего брата, Петя решил выяснить, какое максимальное число могло быть написано на полоске бумаги перед разрезанием. Помогите ему!

Входные данные
Входной файл содержит одну или более строк, каждая из которых содержит последовательность цифр. Количество строк во входном файле не превышает 100, каждая строка содержит от 1 до 100 цифр. Гарантируется, что хотя бы в одной строке первая цифра отлична от нуля.

Выходные данные
Выведите в выходной файл одну строку — максимальное число, которое могло быть написано на полоске перед разрезанием.
*/

#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

typedef struct Line_t {
    unsigned char *string;
    size_t len;
} Line;

/// Struct to store file's information into
typedef struct File_t {
    const char *name;
    FILE *file_dscr;
    struct stat info;
    unsigned char *text;
    size_t lines_cnt;
    Line **lines;
} File;

void Char_get_next_symb(const unsigned char **c) {
    assert(c);

    const unsigned char *cur_c = *c;
    while(isspace(*cur_c) && *cur_c) {
        ++cur_c;
    }
    *c = cur_c;
}

char Char_in_string(const unsigned char c, const unsigned char *str) {
    while (*str) {
        if (c == *str) {
            return 1;
        }
        ++str;
    }
    return 0;
}

int compare_lines_letters(const void *elem1, const void *elem2) {
    const unsigned char *first_c  = ((**(Line* const *)elem1).string);
    const unsigned char *second_c = ((**(Line* const *)elem2).string);

    while (*first_c && *second_c) {
        Char_get_next_symb(&first_c);
        Char_get_next_symb(&second_c);

        if (*first_c != *second_c || (*first_c) * (*second_c) == 0) {
            return (int) *first_c - (int) *second_c;
        }

        ++first_c;
        ++second_c;
    }

    Char_get_next_symb(&first_c);
    Char_get_next_symb(&second_c);
    return (int) *first_c - (int) *second_c;
}

int reverse_compare_lines_letters(const void **elem1, const void **elem2) {
    return -compare_lines_letters(elem1, elem2);
}

void File_destruct(const File *file) {
    assert(file);

    Line **lines_ptr = file->lines;
    for (size_t i = 0; i < file->lines_cnt; ++i) {
        free(*lines_ptr);
        ++lines_ptr;
    }
    free(file->lines);
    free(file->text);
}

int read_lines(File *file) {
    assert(file);

    file->file_dscr = fopen(file->name, "rb");

    fstat(fileno(file->file_dscr), &(file->info));

    file->text = (unsigned char*) calloc((size_t) file->info.st_size + 1, sizeof(char));
    if (!file->text) {
        return -1;
    }

    fread(file->text, (size_t) file->info.st_size, 1, file->file_dscr);
    
    int lines_cnt = 0;
    for (unsigned char *c = file->text; *c; ++c) {
        lines_cnt += *c == '\n';
    }
    file->lines_cnt = (size_t) lines_cnt + 1;

    file->lines = (Line**) calloc((size_t) lines_cnt + 1, sizeof(Line*));
    if (file->lines == NULL) {
        return -1;
    }

    unsigned char *c = file->text;
    lines_cnt = -1;
    int line_len = 0;
    int itrs = 0;
    while (itrs < file->info.st_size && *c) {
        ++lines_cnt;

        file->lines[lines_cnt] = (Line*) calloc(1, sizeof(Line));
        Line *line_ptr = file->lines[lines_cnt];
        if (line_ptr == NULL) {
            return -1;
        }
        line_ptr->string = c;

        while(itrs < file->info.st_size && *c != '\n') {
            ++line_len;
            ++c;
            ++itrs;
            if (*c == '\r') {
                *c = '\0';
            }
        }
        *c = '\0';
        ++c;
        ++itrs;

        line_ptr->len = (size_t) line_len;
        line_len = 0;
    }

    return lines_cnt + 1;
}


int File_construct(File *file, const char *name) {
    assert(file);
    assert(name);

    file->name = name;

    const int ret = read_lines(file);
    file->lines_cnt = (size_t) ret;
    return ret;
}

char String_starts_with(const unsigned char *line, const char *sample) {
    if (!*line || !*sample) {
        return 0;
    }

    while (*line && *sample) {
        if (*line != *sample) {
            return 0;
        }
        ++line;
        ++sample;
    }

    return 1;
}

void print_file(const File *file, const char *fout_name, const char *mode) {
    assert(file);
    assert(fout_name);

    FILE *fout = fopen(fout_name, mode);
    for (size_t i = 0; i < file->lines_cnt; ++i) {
        fprintf(fout, "%s", file->lines[i]->string);
    }

    fclose(fout);
}

// Необходимо понять, какое число больше: first##second или second##first, 
// на основе этого поймём, какую строку нужно использовать раньше. Легко доказать, что 
// в таком случае и отсортированный массив строк даст наибольшее итоговое число

int Line_concat_sort(const Line **first_ptr, const Line **second_ptr) {
    const Line* first = *first_ptr;
    const Line* second = *second_ptr;
 
    char *c1_ptr = first->string;
    char *c2_ptr = second->string;
 
    char c1 = *c1_ptr;
    char c2 = *c2_ptr;
 
    const size_t sum_len = first->len + second->len;
    for (size_t i = 0; i < sum_len; ++i) {
        if (c1 > c2) {
            return -1;
        } else if (c2 > c1) {
            return 1;
        }
 
        c1 = *(++c1_ptr);
        c2 = *(++c2_ptr);
        if (!c1) {
            c1 = *(c1_ptr = second->string);
        }
        if (!c2) {
            c2 = *(c2_ptr = first->string);
        }
    }
 
    return 0;
}

int main() {
    const char *fin_name  = "number.in";
    const char *fout_name = "number.out";
 
    File fin = {};
    File_construct(&fin, fin_name);
 
    qsort(fin.lines, fin.lines_cnt, sizeof(Line*), Line_concat_sort); // Отсортируем строки по ключу, следящему, какая комбинация (AB или BA) больше
 
    print_file(&fin, fout_name, "w");
 
    File_destruct(&fin);
 
    return 0;
}

// O(nlogn) - сортировка. можно учесть длину строк, но она не большая