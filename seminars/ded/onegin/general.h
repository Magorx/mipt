#ifndef KCTF_GENERAL_H
#define KCTF_GENERAL_H

#include <assert.h>

const int KCTF_DEBUG_MODE = 1;

int           DEBUG_NUMBER = 1;   // Just an int for debugging
unsigned char DEBUG_LETTER = 'a'; // Just a char for debugging

#define DEBUG_NUMBER_PRINT() printf("[deb] %d [deb]\n", DEBUG_NUMBER++);
#define DEBUG_LETTER_PRINT() printf("[deb] %c [deb]\n", DEBUG_LETTER++);

#define DEBUG_NUMBER_INCR() DEBUG_NUMBER++;
#define DEBUG_LETTER_INCR() DEBUG_LETTER++;

#define DEBUG if(KCTF_DEBUG_MODE)

// Current project's setting
enum CURRENT_PROJECT_SETTINGS { // special for Onegin
    STROFA_SIZE = 14,
    RHYME_DEPTH = 4
};

//  Errors codes
enum ERRORS {
    ERROR_FILE_NOT_FOUND = -10,
    ERROR_BIG_FILE,
    ERROR_MALLOC_FAIL
};

// Handmade stringview
struct Line {
    unsigned char *string;
    size_t len;
    unsigned char ending[RHYME_DEPTH + 1]; // special for Onegin
};

typedef struct Line Line_t;

// Struct to store file's information into
struct File {
    char *name;
    int file_ptr;
    struct stat info;
    unsigned char *text;
    int lines_cnt;
    Line_t **lines;
};

typedef struct File File_t;

/**
    @brief Quadratic sort

    Sorts array on-place with given comparator

    @param[in] arr array which needs to be sorted
    @param[in] elem_cnt count of elements, [0, elem_cnt) will be sorted
    @param[in] elem_size size of each element in bytes
    @param[in] comp comparator returning an int <0 of elem1<elem2, 0 if elem1=elem2, >0 if elem1>elem2
*/
void qqh_sort(const void *arr, const size_t elem_cnt, const size_t elem_size, int (*comp)(void *elem1, void *elem2));

/**
    @brief Comparator for two lines

    Ignores everything that is not a Russian or English letter

    @param[in] elem1,elem2 elements to compare
    @return an int <0 if elem1<elem2, 0 if elem1=elem2, >0 if elem1>elem2
*/
int compare_lines_letters(const void **elem1, const void **elem2);

/**
    @brief Reversed comparator for two lines

    Ignores everything that is not a Russian or English letter

    @param[in] elem1,elem2 elements to compare
    @return an int >0 if elem1<elem2, 0 if elem1=elem2, 0 if elem1>elem2
*/
int reverse_compare_lines_letters(const void **elem1, const void **elem2);

/**
    @brief Calls all necessary free's

    Kind of destructor for the File structure

    @param[in] file object to be destroyed
*/
void free_memory_file(const File_t *file);

/**
    @brief Reads file

    Estimates file's size and allocates just enough memory + 1 byte for \0, then calls read_lines to fill buffer 'text' and index 'lines'

    @param[in] file object to be read to
    @param[in] name - filename to be read from
    @return 0 if file is read successfully, else error code <0
*/
int read_file(File_t *file, const char *name);

/**
    @brief Reads lines from file

    Stores them into given File_t object

    @param[in] file an object to write into, contains input file name
    @return 0 if file is read successfully, else error code <0
*/
int read_lines(File_t *file);

/**
    @brief Prints file into given file



    @param[in] file file containing text to write
    @param[in] output file name
*/
void print_file(const File_t *file, const char *fout);

/**
    @brief Checks if c is a Russian letter

    .

    @param[in] c char to check
    @return true if c is a Russian letter, else false
*/
char is_russian_letter(const unsigned char c);

/**
    @brief Checks if c is a Russian or an English letter

    .

    @param[in] c char to check
    @return true if c is a Russian or an English letter, else false
*/
char is_letter(const unsigned char c);

/**
    @brief Swaps contains of two ptrs

    .

    @param[in] first,second prts to swap
    @return
*/
void swap_ptrs(void **first, void **second);

/**
    @brief Prints text error message to standard output

    .

    @param[in] error code of error to print
*/
void print_error(int error);


//=============================================================================
//========================== REALISATON =======================================
//=============================================================================


char is_russian_letter(const unsigned char c) {
    return (c >= (unsigned char) '�' && c <= (unsigned char) '�') || (c >= (unsigned char) '�' && c <= (unsigned char) '�');
}

char is_letter(const unsigned char c) {
    return isalpha(c) || is_russian_letter(c);
}

void swap_ptrs(void **first, void **second) {
    void *tmp = *second;
    *second = *first;
    *first = tmp;
}

void qqh_sort(const void *arr, const size_t elem_cnt, const size_t elem_size, int (*comp)(void *elem1, void *elem2)) {
    assert(arr);

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

int compare_lines_letters(const void **elem1, const void **elem2) {
    const unsigned char* str1 = ((Line_t*)(*elem1))->string;
    const unsigned char* str2 = ((Line_t*)(*elem2))->string;

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

int reverse_compare_lines_letters(const void **elem1, const void **elem2) {
    return -compare_lines_letters(elem1, elem2);
}

void free_memory_file(const File_t *file) {
    assert(file);

    Line_t **lines_ptr = file->lines;
    for (int i = 0; i < file->lines_cnt; ++i) {
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

    file->text = calloc(file->info.st_size + 1, sizeof(char));
    if (!file->text) {
        return ERROR_MALLOC_FAIL;
    }
    return file->lines_cnt = read_lines(file);
}

int read_lines(File_t *file) {
    assert(file);

    DEBUG {printf("Working with [%s] file\n", file->name);}

    file->file_ptr = open(file->name, O_BINARY);
    if (!file->file_ptr) {
        return ERROR_FILE_NOT_FOUND;
    }

    read(file->file_ptr, file->text, file->info.st_size);
    int lines_cnt = 0;
    for (unsigned char *c = file->text; *c; ++c) {
        lines_cnt += *c == '\n';
    }
    file->lines = calloc(lines_cnt, sizeof(Line_t*));
    if (!file->lines) {
        return ERROR_MALLOC_FAIL;
    }

    unsigned char *c = file->text;
    lines_cnt = -1;
    int line_len = 0;
    while (*c) {
        ++lines_cnt;

        file->lines[lines_cnt] = calloc(1, sizeof(Line_t));
        if (!file->lines[lines_cnt]) {
            return ERROR_MALLOC_FAIL;
        }
        Line_t *line_ptr = file->lines[lines_cnt];
        line_ptr->string = c;

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
    file->lines_cnt = lines_cnt;

    return lines_cnt + 1;
}

void print_file(const File_t *file, const char *fout_name) {
    assert(file);
    assert(fout_name);

    FILE *fout = fopen(fout_name, "w");
    for (int i = 0; i < file->lines_cnt; ++i) {
        fprintf(fout, "%s\n", file->lines[i]->string);
    }

    fclose(fout);
}

void print_error(int error) {
    if (error == 0) {
        printf(""); // wtf?
        return;
    }

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

#endif // KCTF_GENERAL_H