/**
    \file
    \brief General functions to be used in all projects
*/

#ifndef KCTF_GENERAL_H
#define KCTF_GENERAL_H

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>

//<KCTF> Everyday_staff =======================================================

const int KCTF_DEBUG_LEVEL = 1; ///< Just a mode for debugging

int           DEBUG_NUMBER = 1;   ///< Just an int for debugging
unsigned char DEBUG_LETTER = 'a'; ///< Just a char for debugging

//Just Joking

#define DEBUG_NUMBER_PRINT() printf("[deb] %d [deb]\n", DEBUG_NUMBER++);
#define DEBUG_LETTER_PRINT() printf("[deb] %c [deb]\n", DEBUG_LETTER++);

#define DEBUG_NUMBER_INCR() DEBUG_NUMBER++;
#define DEBUG_LETTER_INCR() DEBUG_LETTER++;

#define DEBUG(LEVEL) if (LEVEL <= KCTF_DEBUG_LEVEL)

const int INT_P = -7777777; /// Poison int
const size_t SIZE_T_P = -1; /// Poison size_t

///  Return codes
enum RETURN_CODES {
    ERROR_ERROR = -7777777,
    ERROR_FILE_NOT_FOUND = -20,
    ERROR_BIG_FILE,
    ERROR_MALLOC_FAIL,
    ERROR_NULL_OBJECT,
    ERROR_NO_RET_CODE,
    ERROR_BAD_ARGS,
    ERROR_CHECK_UPPER_ASSERT = -1,

    NULL_OBJ_OK = 0,
    RET_OK = 0
};

//=============================================================================
//<KCTF> Handmade asserts =====================================================
const int KCTF_ASSERT_LOUDNESS = 1;
const int FATAL_ERROR = 2;
const int CHECK_ERROR = 1;

#define FULL_ASSERT(expr, err_name, loudness, cur_loudness, droptable, ERROR)        \
    do {                                                                             \
        int ret = (expr);                                                            \
        if ((ERROR || !ret) && (loudness) >= (cur_loudness)) {                       \
             if (ret == 0) {                                                         \
             printf("[ERR]<assert>: [erro_name](%s)\n", "CHECK_UPPER_ASSERT");       \
             printf("[   ]<      >: [erro_code](%d)\n", ERROR_CHECK_UPPER_ASSERT);   \
             }                                                                       \
             else {                                                                  \
             printf("[ERR]<assert>: [erro_name](%s)\n", err_name);                   \
             printf("[   ]<      >: [erro_code](%d)\n", ret);                        \
             }                                                                       \
             printf("[   ]<      >: [file_name](%s)\n", __FILE__);                   \
             printf("[   ]<      >: [func_name](%s)\n", __FUNCTION__);               \
             printf("[   ]<      >: [line_indx](%d)\n", __LINE__);                   \
        }                                                                            \
        if (ERROR || !ret) {                                                         \
            if (droptable) { exit   (ERROR_CHECK_UPPER_ASSERT); }                    \
            else           { return (ERROR_CHECK_UPPER_ASSERT); }                    \
        }                                                                            \
    } while(0)

#define YESDROP_ASSERT(expr, err_name, loudness, cur_loudness) FULL_ASSERT(expr, err_name, loudness, cur_loudness, 1, 0)
#define LOUDSET_ASSERT(expr, err_name, loudness) YESDROP_ASSERT(expr, err_name, loudness, KCTF_ASSERT_LOUDNESS)
#define ERRCDNM_ASSERT(expr, err_name) LOUDSET_ASSERT(expr, err_name, KCTF_ASSERT_LOUDNESS)

#define         ASSERT(expr) ERRCDNM_ASSERT(expr, #expr)
#define RETURN_ERROR_ASSERT(expr) FULL_ASSERT(expr, #expr, KCTF_ASSERT_LOUDNESS, KCTF_ASSERT_LOUDNESS, 0, 1)
#define CHECK_ERROR_ASSERT(expr) FULL_ASSERT(expr, #expr, KCTF_ASSERT_LOUDNESS, KCTF_ASSERT_LOUDNESS, 0, 0)

//=============================================================================
//<KCTF> Handmade_overloads ===================================================

#define CONCAT(a, c) a ## _ ## c
#define OVERLOAD(func, type) CONCAT(func, type)

//=============================================================================
//<KCTF> Handmade_hash ========================================================

const long long HASH_MODULE = 1000000007;
const long long BASE = 257;

#define get_hash(struct) do_hash(struct, sizeof(struct))

long long do_hash(void *memptr, size_t size_in_bytes) {
    assert(memptr);
    long long ret = 1;
    char *ptr = (char*) memptr;
    for (size_t i = 0; i < size_in_bytes; ++i) {
        ret = (ret * BASE + ptr[i]) % HASH_MODULE;
    }
    return ret;
}

//=============================================================================
/// Handmade stringview =======================================================
struct Line {
    unsigned char *string;
    size_t len;
    int index; // for debug !#!@#@!#@!#@!#
};

/// Typedef for Line
typedef struct Line Line_t;

/// Struct to store file's information into
struct File {
    const char *name;
    size_t file_dscr;
    struct stat info;
    unsigned char *text;
    size_t lines_cnt;
    Line_t **lines;
};

/// Typedef for File
typedef struct File File_t;

/**
    \brief Quadratic sort

    Sorts array on-place with given comparator

    \param[in] arr array which needs to be sorted
    \param[in] elem_cnt count of elements, [0, elem_cnt) will be sorted
    \param[in] elem_size size of each element in bytes
    \param[in] comp comparator returning an int <0 of elem1<elem2, 0 if elem1=elem2, >0 if elem1>elem2
*/
void qqh_sort(void *arr, const size_t elem_cnt, const size_t elem_size, int (*comp)(const void *elem1, const void *elem2));

/**
    \brief Comparator for two lines

    Ignores everything that is not a Russian or English letter

    \param[in] elem1,elem2 elements to compare
    \return an int <0 if elem1<elem2, 0 if elem1=elem2, >0 if elem1>elem2
*/
int compare_lines_letters(const void *elem1, const void *elem2);

/**
    \brief Reversed comparator for two lines

    Ignores everything that is not a Russian or English letter

    \param[in] elem1,elem2 elements to compare
    \return an int >0 if elem1<elem2, 0 if elem1=elem2, 0 if elem1>elem2
*/
int reverse_compare_lines_letters(const void **elem1, const void **elem2);

/**
    \brief Calls all necessary free's

    Kind of destructor for the File structure

    \param[in] file object to be destroyed
*/
void free_memory_file(const File_t *file);

/**
    \brief Reads file

    Estimates file's size and allocates just enough memory + 1 byte for \0, then calls read_lines to fill buffer 'text' and index 'lines'

    \param[in] file object to be read to
    \param[in] name - filename to be read from
    \return 0 if file is read successfully, else error code <0
*/
int read_file(File_t *file, const char *name);

/**
    \brief Reads lines from file

    Stores them into given File_t object

    \param[in] file an object to write into, contains input file name
    \return 0 if file is read successfully, else error code <0
*/
int read_lines(File_t *file);

/**
    \brief Prints file into given file



    \param[in] file file containing text to write
    \param[in] output file name
*/
void print_file(const File_t *file, const char *fout_name, const char *mode);


/**
    \brief Checks if c is a Russian or an English letter

    .

    \param[in] c char to check
    \return true if c is a Russian or an English letter, else false
*/
int is_countable(const unsigned char c);

/**
    \brief Swaps contains of two ptrs

    .

    \param[in] first,second prts to swap
    \return
*/
void swap_ptrs(void **first, void **second);

/**
    \brief Sets *c on next position of the same string, where is_countable returns true.

    If there are no such left, *c will be pointer to the end of the string ('\0')

    \param[in] first,second prts to swap
    \return
*/
void get_next_letter(unsigned char **c);

/**
    \brief Prints text error message to standard output

    .

    \param[in] error code of error to print
*/
void print_error(int error);


//=============================================================================
// IMPLEMENTATION
//=============================================================================

int is_countable(const unsigned char c) {
    return isalnum(c);
}

void swap_ptrs(void **first, void **second) {
    assert(first);
    assert(second);

    void *tmp = *second;
    *second = *first;
    *first = tmp;
}

void qqh_sort(void *input_arr, const size_t elem_cnt, const size_t elem_size, int (*comp)(const void *elem1, const void *elem2)) {
    assert(input_arr);
    assert(comp);

    char *arr = (char*) input_arr;

    for (size_t i = 0; i < elem_cnt; ++i) {
        for (size_t j = 0; j < elem_cnt - 1; ++j) {
            void *first = arr + j * elem_size;
            void *second = arr + (j + 1) * elem_size;
            if (comp(first, second) > 0) {
                swap_ptrs((void**) first, (void**) second);
            }
        }
    }
}

void get_next_letter(unsigned char **c) {
    assert(c);

    unsigned char *cur_c = *c;
    while(!is_countable(*cur_c) && *cur_c) {
        ++cur_c;
    }
    *c = cur_c;
}

int compare_lines_letters(const void *elem1, const void *elem2) {
    unsigned char *first_c  = ((**(Line_t**)elem1).string);
    unsigned char *second_c = ((**(Line_t**)elem2).string);

    while (*first_c && *second_c) {
        get_next_letter(&first_c);
        get_next_letter(&second_c);

        if (*first_c != *second_c || (*first_c) * (*second_c) == 0) {
            return (int) *first_c - (int) *second_c;
        }

        ++first_c;
        ++second_c;
    }

    get_next_letter(&first_c);
    get_next_letter(&second_c);
    return (int) *first_c - (int) *second_c;
}

int reverse_compare_lines_letters(const void **elem1, const void **elem2) {
    return -compare_lines_letters(elem1, elem2);
}

void free_memory_file(const File_t *file) {
    assert(file);

    Line_t **lines_ptr = file->lines;
    for (size_t i = 0; i < file->lines_cnt; ++i) {
        free(*lines_ptr);
        ++lines_ptr;
    }
    free(file->lines);
    free(file->text);
}

int read_file(File_t *file, const char *name) {
    assert(file);
    assert(name);

    file->name = name;
    stat(name, &(file->info));

    file->text = (unsigned char*) calloc(file->info.st_size + 1, sizeof(char));
    if (!file->text) {
        return ERROR_MALLOC_FAIL;
    }
    return file->lines_cnt = read_lines(file);
}

int read_lines(File_t *file) {
    assert(file);

    DEBUG(1) {printf("Working with [%s] file\n", file->name);}

    file->file_dscr = open(file->name, O_BINARY);

    read(file->file_dscr, file->text, file->info.st_size);
    int lines_cnt = 0;
    for (unsigned char *c = file->text; *c; ++c) {
        lines_cnt += *c == '\n';
    }

    file->lines = (Line_t**) calloc(lines_cnt, sizeof(Line_t*));
    if (file->lines == NULL) {
        return ERROR_MALLOC_FAIL;
    }

    unsigned char *c = file->text;
    lines_cnt = -1;
    int line_len = 0;
    while (*c) {
        ++lines_cnt;

        file->lines[lines_cnt] = (Line_t*) calloc(1, sizeof(Line_t));
        Line_t *line_ptr = file->lines[lines_cnt];
        if (line_ptr == NULL) {
            return ERROR_MALLOC_FAIL;
        }
        line_ptr->string = c;
        line_ptr->index = lines_cnt;

        while(*c != '\n') {
            ++line_len;
            ++c;
            if (*c == '\r') {
                *c = '\0';
            }
        }
        *c = '\0';
        ++c;

        line_ptr->len = line_len;
        line_len = 0;
    }
    file->lines_cnt = lines_cnt + 1;

    return lines_cnt + 1;
}

void print_file(const File_t *file, const char *fout_name, const char *mode) {
    assert(file);
    assert(fout_name);

    FILE *fout = fopen(fout_name, mode);
    for (size_t i = 0; i < file->lines_cnt; ++i) {
        fprintf(fout, "%s\n", file->lines[i]->string);
    }

    fclose(fout);
}

void print_error(int error) {
    if (error == 0) {
        return;
    }

    if (error == ERROR_FILE_NOT_FOUND) {
        printf("[ERR] File not found!\n");
    } else if (error == ERROR_BIG_FILE) {
        printf("[ERR] Can't handle such a big file!\n");
    } else if (error == ERROR_MALLOC_FAIL) {
        printf("[ERR] Can't allocate memory\n");
    } else {
        printf("[ERR](~!~)WERRORHUTGEERRORF(~!~)[ERR]\n");
    }
}


// UNIT TESTS

int utest_compare_lines_letters() {
    //srand(time(NULL));
    File_t file = {};
    read_file(&file, "utest_compare_lines_letters.txt");
    for (size_t itter = 0; itter < 1000; ++itter) {
        for (size_t i = 0; i < file.lines_cnt / 2; ++i) {
            const size_t x = rand() % file.lines_cnt;
            const size_t y = rand() % file.lines_cnt;
            Line_t *tmp = file.lines[x];
            file.lines[x] = file.lines[y];
            file.lines[y] = tmp;
        }

        qsort(file.lines, file.lines_cnt, sizeof(Line_t*), compare_lines_letters);

        for (size_t i = 0; i < file.lines_cnt - 1; ++i) {
            if (file.lines[i]->index > file.lines[i + 1]->index && compare_lines_letters(&file.lines[i], &file.lines[i + 1])) {
                printf("[ERR] \"%s\" > \"%s\"\n", file.lines[i]->string, file.lines[i + 1]->string);
                printf("[ ! ] indexes: %d > %d\n", file.lines[i]->index, file.lines[i + 1]->index);
                DEBUG(2) {
                    printf("====\n");
                    for (size_t j = 0; j < file.lines_cnt; ++j) {
                        printf("%s\n", file.lines[j]->string);
                    }
                }
            }
        }
    }

    return 0;
}

//<KCTF>[SUPER_SIMPLE] ========================================================

long long int min(long long int a, long long int b) {
    return a > b ? b : a;
}

long long int max(long long int a, long long int b) {
    return a < b ? b : a;
}

#endif // KCTF_GENERAL_H
