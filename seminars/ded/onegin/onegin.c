#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

const int MAXSTRLEN = 100;
const int MAXSTRS = 10000;

char is_russian_letter(unsigned char c) {
    return (c >= (unsigned char) '�' && c <= (unsigned char) '�') || (c >= (unsigned char) '�' && c <= (unsigned char) '�');
}

char is_letter(unsigned char c) {
    return isalpha(c) || is_russian_letter(c);
}

int compare_strings(const void **elem1, const void **elem2) {
    unsigned char* str1 = *elem1;
    unsigned char* str2 = *elem2;

    int i = 0;
    int j = 0;
    while (str1[i] && str2[j]) {
        while (!is_letter(str1[i]) && str1[i]) {
            i++;
        }
        while (!is_letter(str2[j]) && str2[j]) {
            j++;
        }

        if (str1[i] != str2[i] || str1[i] * str2[i] == 0) {
            return str1[i] - str2[j];
        }

        i++;
        j++;
    }
    return str1[i] - str2[j];
}

int reverse_compare_strings(const void **elem1, const void **elem2) {
    return -compare_strings(elem1, elem2);
}

void char_test(unsigned char x) {
    printf("%c %d\n", x, x);
}

void free_lines_memory(unsigned char **lines) {
    unsigned char **lines_ptr = lines;
    for (int i = 0; i < MAXSTRS; ++i) {
        free(*lines_ptr);
        ++lines_ptr;
    }
    free(lines);
}

int main(const int argc, const char **argv) {
    setlocale(LC_ALL,"Russian");

    unsigned char **lines = calloc(MAXSTRS, sizeof(char*));
    unsigned char **lines_ptr = lines;
    for (int i = 0; i < MAXSTRS; ++i) {
        *lines_ptr = calloc(MAXSTRLEN, sizeof(char*));
        ++lines_ptr;
    }

    char *fin_name = "onegin.txt";
    char *fout_name = "oneginized.txt";
    if (argc > 1) {
        fin_name = argv[1];
    }
    if (argc > 2) {
        fout_name = argv[2];
    }

    FILE *fin = fopen(fin_name, "r");
    if (!fin) {
        printf("File '%s' not found!\n", fin_name);
        free_lines_memory(lines);
        return 0;
    }

    lines_ptr = lines;
    int str_cnt = 0;
    printf("%d\n", fin);
    while (fgets(*(lines_ptr++), sizeof(char) * MAXSTRLEN, fin)) {
        str_cnt += 1;
        if (str_cnt == MAXSTRS - 1) {
            printf("Can't handle such a big file!\n");
            free_lines_memory(lines);
            return 0;
        }
    }

    qsort(lines, str_cnt, sizeof(char*), compare_strings);
    printf("%d lines are sorted!\n", str_cnt);

    FILE *fout = fopen(fout_name, "w");
    for (int i = 0; i < str_cnt; ++i) {
        fprintf(fout, "%s", lines[i]);
    }

    fclose(fout);
    free_lines_memory(lines);

    return 0;

}
 ;
