/**
    \file
    \brief General functions to be used in all projects
*/

/* todo
    + formated_print() for [what]<who>: xxx
*/

#ifndef KCTF_GENERAL_H
#define KCTF_GENERAL_H

// Thanks @ded32 for these warning!
// <Ded32> WARNINGS ===========================================================
#if  defined (__GNUC__)

    #pragma GCC diagnostic ignored     "-Wpragmas"

    #pragma GCC diagnostic warning     "-Wall"
    #pragma GCC diagnostic warning     "-Weffc++"
    #pragma GCC diagnostic warning     "-Wextra"

    #pragma GCC diagnostic warning     "-Waggressive-loop-optimizations"
    #pragma GCC diagnostic warning     "-Walloc-zero"
    #pragma GCC diagnostic warning     "-Walloca"
    #pragma GCC diagnostic warning     "-Walloca-larger-than=8192"
    #pragma GCC diagnostic warning     "-Warray-bounds"
    #pragma GCC diagnostic warning     "-Wcast-align"
    #pragma GCC diagnostic warning     "-Wcast-qual"
    #pragma GCC diagnostic warning     "-Wchar-subscripts"
    #pragma GCC diagnostic warning     "-Wconditionally-supported"
    #pragma GCC diagnostic warning     "-Wconversion"
    #pragma GCC diagnostic warning     "-Wctor-dtor-privacy"
    #pragma GCC diagnostic warning     "-Wdangling-else"
    #pragma GCC diagnostic warning     "-Wduplicated-branches"
    #pragma GCC diagnostic warning     "-Wempty-body"
    #pragma GCC diagnostic warning     "-Wfloat-equal"
    #pragma GCC diagnostic warning     "-Wformat-nonliteral"
    #pragma GCC diagnostic warning     "-Wformat-overflow=2"
    #pragma GCC diagnostic warning     "-Wformat-security"
    #pragma GCC diagnostic warning     "-Wformat-signedness"
    #pragma GCC diagnostic warning     "-Wformat-truncation=2"
    #pragma GCC diagnostic warning     "-Wformat=2"
    #pragma GCC diagnostic warning     "-Wlarger-than=8192"
    #pragma GCC diagnostic warning     "-Wlogical-op"
    #pragma GCC diagnostic warning     "-Wnarrowing"
    #pragma GCC diagnostic warning     "-Wnon-virtual-dtor"
    #pragma GCC diagnostic warning     "-Wnonnull"
    #pragma GCC diagnostic warning     "-Wopenmp-simd"
    #pragma GCC diagnostic warning     "-Woverloaded-virtual"
    #pragma GCC diagnostic warning     "-Wpacked"
    #pragma GCC diagnostic warning     "-Wpointer-arith"
    #pragma GCC diagnostic warning     "-Wredundant-decls"
    #pragma GCC diagnostic warning     "-Wrestrict"
    #pragma GCC diagnostic warning     "-Wshadow"
    #pragma GCC diagnostic warning     "-Wsign-promo"
    #pragma GCC diagnostic warning     "-Wstack-usage=8192"
    #pragma GCC diagnostic warning     "-Wstrict-aliasing"
    #pragma GCC diagnostic warning     "-Wstrict-null-sentinel"
    #pragma GCC diagnostic warning     "-Wstrict-overflow=2"
    #pragma GCC diagnostic warning     "-Wstringop-overflow=4"
    #pragma GCC diagnostic warning     "-Wsuggest-attribute=noreturn"
    #pragma GCC diagnostic warning     "-Wsuggest-final-methods"
    #pragma GCC diagnostic warning     "-Wsuggest-final-types"
    #pragma GCC diagnostic warning     "-Wsuggest-override"
    #pragma GCC diagnostic warning     "-Wswitch-default"
    #pragma GCC diagnostic warning     "-Wswitch-enum"
    #pragma GCC diagnostic warning     "-Wsync-nand"
    #pragma GCC diagnostic warning     "-Wundef"
    #pragma GCC diagnostic warning     "-Wunused"
    #pragma GCC diagnostic warning     "-Wvarargs"
    #pragma GCC diagnostic warning     "-Wvariadic-macros"
    #pragma GCC diagnostic warning     "-Wvla-larger-than=8192"

    #pragma GCC diagnostic error       "-Wsizeof-array-argument"

    #pragma GCC diagnostic ignored     "-Winline"
    #pragma GCC diagnostic ignored     "-Wliteral-suffix"
    #pragma GCC diagnostic ignored     "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored     "-Wnonnull-compare"
    #pragma GCC diagnostic ignored     "-Wold-style-cast"
    #pragma GCC diagnostic ignored     "-Wunreachable-code"
    #pragma GCC diagnostic ignored     "-Wunused-const-variable"
    #pragma GCC diagnostic ignored     "-Wunused-function"

    #pragma GCC diagnostic warning     "-Wpragmas"
#endif

//=============================================================================

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

typedef int8_t Byte;

//<KCTF> Everyday_staff =======================================================

const int KCTF_DEBUG_LEVEL = 1; ///< Just a mode for debugging

int           DEBUG_NUMBER = 1;   ///< Just an int for debugging
unsigned char DEBUG_LETTER = 'a'; ///< Just a char for debugging

#define DEBUG_NUMBER_PRINT() printf("[deb] %d [deb]\n", DEBUG_NUMBER++);
#define DEBUG_LETTER_PRINT() printf("[deb] %c [deb]\n", DEBUG_LETTER++);

#define DEBUG_PRINTF() DEBUG_NUMBER_PRINT();

#define DEBUG_NUMBER_INCR() DEBUG_NUMBER++;
#define DEBUG_LETTER_INCR() DEBUG_LETTER++;

#define DEBUG(LEVEL) if (LEVEL <= KCTF_DEBUG_LEVEL)

const size_t STANDART_INIT_SIZE = 32;

const long KCTF_POISON = -7777777;
const int INT_P = -7777777; /// Poison int
const size_t SIZE_T_P = 7777777; /// Poison size_t

///  Return codes
enum RETURN_CODES {
    ERROR_ERROR = -7777777,
    ERROR_FILE_NOT_FOUND = -20,
    ERROR_BIG_FILE,
    ERROR_MALLOC_FAIL,
    ERROR_REALLOC_FAIL,
    ERROR_NULL_OBJECT,
    ERROR_NO_RET_CODE,
    ERROR_BAD_ARGS,

    ERROR_BAD_CANARY,
    ERROR_BAD_HASH,

    ERROR_CHECK_UPPER_VERIFY = -1,

    NULL_OBJ_OK = 0,
    RET_OK = 0,
    OK = 0
};

#define PRINTF_ERROR printf("EROR HAPANED\n")

//=============================================================================
//<KCTF> Handmade asserts =====================================================

const int KCTF_VERIFY_LOUDNESS = 1;
const int FATAL_ERROR = 2;
const int CHECK_ERROR = 1;

#define FULL_VERIFY(expr, err_name, loudness, cur_loudness, droptable, ERROR)       \
    do {                                                                            \
        int ret = (expr);                                                           \
        if ((ERROR || !ret) && (loudness) >= (cur_loudness)) {                      \
            printf("[ERR]<assert>: [erro_erro](%s)\n", err_name);                   \
            if (ret == 0) {                                                         \
            printf("[   ]<      >: [erro_code](%X)\n", (unsigned)0xDED);            \
            } else {                                                                \
            printf("[   ]<      >: [erro_code](%d)\n", ret);                        \
            }                                                                       \
            printf("[   ]<      >: [file_name](%s)\n", __FILE__);                   \
            printf("[   ]<      >: [func_name](%s)\n", __FUNCTION__);               \
            printf("[   ]<      >: [line_indx](%d)\n", __LINE__);                   \
            }                                                                       \
        if (ERROR || !ret) {                                                        \
            if (droptable) { exit   (ERROR_CHECK_UPPER_VERIFY); }                   \
            else           { return (ERROR_CHECK_UPPER_VERIFY); }                   \
        }                                                                           \
    } while(0)

#define VERIFY_YESDROP(expr, err_name, loudness, cur_loudness) FULL_VERIFY(expr, err_name, loudness, cur_loudness, 1, 0)
#define VERIFY_LOUDSET(expr, err_name, loudness) VERIFY_YESDROP(expr, err_name, loudness, KCTF_VERIFY_LOUDNESS)
#define VERIFY_ERRCDNM(expr, err_name) VERIFY_LOUDSET(expr, err_name, KCTF_VERIFY_LOUDNESS)

#define VERIFY(expr) VERIFY_ERRCDNM(expr, #expr)
#define VERIFY_OK(expr) VERIFY((expr) == OK)

#define RETURN_ERROR_VERIFY(expr) FULL_VERIFY(expr, #expr, KCTF_VERIFY_LOUDNESS, KCTF_VERIFY_LOUDNESS, 0, 1)

#ifdef VERIFY_BOMB
#define RETURNING_VERIFY(expr) FULL_VERIFY(expr, #expr, KCTF_VERIFY_LOUDNESS, KCTF_VERIFY_LOUDNESS, 1, 0)
#else
#define RETURNING_VERIFY(expr) FULL_VERIFY(expr, #expr, KCTF_VERIFY_LOUDNESS, KCTF_VERIFY_LOUDNESS, 0, 0)
#endif

#define RETURNING_VERIFY_OK(expr) RETURNING_VERIFY((expr) == OK)

//=============================================================================
//<KCTF> Handmade_overloads ===================================================

#define CONCAT(a, c) a ## _ ## c
#define OVERLOAD(func, type) CONCAT(func, type)

//=============================================================================
//<KCTF> Handmade_hash ========================================================

const long long HASH_MODULE = 1000000007;
const long long BASE = 257;

#define get_hash(struct) do_hash(struct, sizeof(struct))
long long do_hash(const void *memptr, size_t size_in_bytes);

long long do_hash(const void *memptr, size_t size_in_bytes) {
    assert(memptr);
    long long ret = 1;
    const char *ptr = (const char*) memptr;
    for (size_t i = 0; i < size_in_bytes; ++i) {
        ret = (ret * BASE + ptr[i]) % HASH_MODULE;
    }
    return ret;
}

//=============================================================================
//<KCTF> Buffer work ==========================================================

void *realloc_buffer(void *buffer, const size_t cur_len, const double coef);
void *realloc_buffer(void *buffer, const size_t cur_len, const double coef) {
    const size_t new_size = (size_t) ((double) cur_len * coef);
    void *new_ptr = realloc(buffer, new_size);
    return new_ptr;
}

//=============================================================================
//<KCTF> Byte output ==========================================================

typedef struct ByteOP_t {
    size_t capacity;
    size_t size;
    Byte *buffer;
    Byte *cur_ptr;
} ByteOP;

ByteOP *new_ByteOP(const size_t size) {
    ByteOP *bop = calloc(sizeof(ByteOP), 1);
    if (!bop) {
        return NULL;
    }

    bop->buffer = calloc(size, 1);
    if (!bop->buffer) {
        free(bop);
        return NULL;
    }
    bop->cur_ptr = bop->buffer;
    bop->capacity = size;
    bop->size = 0;

    return bop;
}

int delete_ByteOP(ByteOP *cake) {
    free(cake->buffer);

    cake->buffer = (Byte*) KCTF_POISON;
    cake->cur_ptr = (Byte*) KCTF_POISON;
    cake->capacity = (size_t) KCTF_POISON;
    cake->size = (size_t) KCTF_POISON;

    free(cake);

    return 0;
}

int ByteOP_realloc_up(ByteOP *cake) {
    assert(cake);
    void *new_ptr = realloc_buffer(cake->buffer, cake->capacity, 2);
    if (!new_ptr) {
        return ERROR_REALLOC_FAIL;
    }

    long int shift = cake->cur_ptr - cake->buffer;
    cake->buffer = (Byte*) new_ptr;
    cake->cur_ptr = cake->buffer + shift;
    cake->capacity = cake->capacity * 2;

    return 0;
}

int ByteOP_put(ByteOP *cake, const void *src, const size_t size) {
    VERIFY(cake != NULL);
    if (cake->size + size >= cake->capacity) {
        VERIFY_OK(ByteOP_realloc_up(cake));
    }

    memcpy(cake->cur_ptr, src, size);
    cake->cur_ptr += size;
    cake->size += size;

    return 0;
}

int ByteOP_put_byte(ByteOP *cake, const Byte src) {
    VERIFY(cake != NULL);
    ByteOP_put(cake, &src, sizeof(Byte));
    return 0;
}

int ByteOP_put_int(ByteOP *cake, const int src) {
    VERIFY(cake != NULL);
    ByteOP_put(cake, &src, sizeof(int));
    return 0;
}

int ByteOP_put_long(ByteOP *cake, const long src) {
    VERIFY(cake != NULL);
    ByteOP_put(cake, &src, sizeof(long));
    return 0;
}

int ByteOP_put_unsigned(ByteOP *cake, const unsigned int src) {
    VERIFY(cake != NULL);
    ByteOP_put(cake, &src, sizeof(unsigned int));
    return 0;
}

int ByteOP_put_double(ByteOP *cake, const double src) {
    VERIFY(cake != NULL);
    ByteOP_put(cake, &src, sizeof(double));
    return 0;
}

int ByteOP_to_file(const ByteOP *cake, const char* filename) {
    VERIFY(cake != NULL);
    FILE *fout = fopen(filename, "wb");
    fwrite(cake->buffer, sizeof(Byte), cake->size, fout);
    fclose(fout);

    return 0;
}

//=============================================================================
///<KCTF> Handmade stringview =================================================
typedef struct Line_t {
    unsigned char *string;
    size_t len;
} Line;

/// Struct to store file's information into
typedef struct File_t {
    const char *name;
    int file_dscr;
    struct stat info;
    unsigned char *text;
    size_t lines_cnt;
    Line **lines;
} File;

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
void File_destruct(const File *file);

/**
    \brief Reads file

    Estimates file's size and allocates just enough memory + 1 byte for \0, then calls read_lines to fill buffer 'text' and index 'lines'

    \param[in] file object to be read to
    \param[in] name - filename to be read from
    \return 0 if file is read successfully, else error code <0
*/
int File_construct(File *file, const char *name);

/**
    \brief Reads lines from file

    Stores them into given File object

    \param[in] file an object to write into, contains input file name
    \return 0 if file is read successfully, else error code <0
*/
int read_lines(File *file);

/**
    \brief Prints file into given file



    \param[in] file file containing text to write
    \param[in] output file name
*/
void print_file(const File *file, const char *fout_name, const char *mode);


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
void get_next_letter(const unsigned char **c);

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

void get_next_letter(const unsigned char **c) {
    assert(c);

    const unsigned char *cur_c = *c;
    while(!is_countable(*cur_c) && *cur_c) {
        ++cur_c;
    }
    *c = cur_c;
}

int compare_lines_letters(const void *elem1, const void *elem2) {
    const unsigned char *first_c  = ((**(Line* const *)elem1).string);
    const unsigned char *second_c = ((**(Line* const *)elem2).string);

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

int File_construct(File *file, const char *name) {
    assert(file);
    assert(name);

    file->name = name;
    stat(name, &(file->info));

    file->text = (unsigned char*) calloc((size_t) file->info.st_size + 1, sizeof(char));
    if (!file->text) {
        return ERROR_MALLOC_FAIL;
    }

    const int ret = read_lines(file);
    file->lines_cnt = (size_t) ret;
    return ret;
}


int read_lines(File *file) {
    assert(file);

    file->file_dscr = open(file->name, O_RDONLY);

    read(file->file_dscr, file->text, (size_t) file->info.st_size);
    
    int lines_cnt = 0;
    for (unsigned char *c = file->text; *c; ++c) {
        lines_cnt += *c == '\n';
    }
    file->lines_cnt = (size_t) lines_cnt + 1;

    file->lines = (Line**) calloc((size_t) lines_cnt + 1, sizeof(Line*));
    if (file->lines == NULL) {
        return ERROR_MALLOC_FAIL;
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
            return ERROR_MALLOC_FAIL;
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

char Line_starts_with(const unsigned char *line, const char *sample);
char Line_starts_with(const unsigned char *line, const char *sample) {
    while (*line && *sample) {
        if (*line != *sample) {
            return 0;
        }
        ++line;
        ++sample;
    }

    return 1;
}

char Line_verify_min_len(const Line *line, const size_t min_length);
char Line_verify_min_len(const Line *line, const size_t min_length) {
    if (line->len >= min_length) {
        return 1;
    } else {
        printf("[ERR]<min_len>: \"%s\" is not of the min len %zu\n", line->string, min_length);
        VERIFY(!"Line is not big enough");
        return 0;
    }
}

void print_file(const File *file, const char *fout_name, const char *mode) {
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

//<KCTF>[SUPER_SIMPLE] ========================================================
long long int min(long long int a, long long int b);
long long int min(long long int a, long long int b) {
    return a > b ? b : a;
}

long long int max(long long int a, long long int b);
long long int max(long long int a, long long int b) {
    return a < b ? b : a;
}

#endif // KCTF_GENERAL_H
