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
        calculate_ending(fin.lines[i], RHYME_DEPTH);
    }

    qsort(fin.lines, fin.lines_cnt, sizeof(Line_t*), compare_lines_letters);
    printf("%d lines are read!\n", fin.lines_cnt);
    print_file(&fin, fout_name);

    unsigned int buffer[STROFA_SIZE];
    gen_strofa(fin.lines, fin.lines_cnt, buffer, RHYME_DEPTH);
    for (int i = 0; i < STROFA_SIZE; ++i) {
        printf("%s\n", fin.lines[buffer[i]]->string);
    }

    free_memory_file(&fin);

    return 0;

}
