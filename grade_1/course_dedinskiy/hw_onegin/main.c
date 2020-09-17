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
#include "onegin.h"

#define TEST

int main(const int argc, char **argv) {
    setlocale(LC_CTYPE,"Russian");

    #ifdef TEST
        utest_compare_lines_letters();
    #else

    char *fin_name  = "onegin.txt";
    char *fout_name = "oneginized.txt";
    if (argc > 1) {
        fin_name = argv[1];
    }
    if (argc > 2) {
        fout_name = argv[2];
    }

    File_t fin = {};
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
        calculate_ending(fin.lines[i], RHYME_DEPTH);
    }

    qsort(fin.lines, fin.lines_cnt, sizeof(Line_t*), compare_lines_letters);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat" // Ignoring unexistance of %z in older versions of compiler
    #pragma GCC diagnostic ignored "-Wformat-extra-args" // Ignoring printf warning
    printf("%zu lines are read!\n", fin.lines_cnt);
    #pragma GCC diagnostic pop
    #pragma GCC diagnostic pop

    print_file(&fin, fout_name);

    unsigned int buffer[STROFA_SIZE];
    gen_strofa((const Line_t**)fin.lines, fin.lines_cnt, buffer, RHYME_DEPTH);
    for (int i = 0; i < STROFA_SIZE; ++i) {
        printf("%s\n", fin.lines[buffer[i]]->string);
    }

    free_memory_file(&fin);
    #endif

    return 0;

}
