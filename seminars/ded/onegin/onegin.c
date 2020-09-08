#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

const int MAXSTRLEN = 100;
const int MAXSTRS   = 10000;

const char ERROR = -1;

int compare_strings(const void **elem1, const void **elem2);
int reverse_compare_strings(const void **elem1, const void **elem2);
void free_memory(unsigned char **lines, char *fin_name, char *fout_name);
int read_lines(char *file_name, unsigned char **lines);
void print_lines(char *file_names, unsigned char **lines, int lines_cnt);

int main(const int argc, const char **argv) {
    setlocale(LC_ALL,"Russian");

    unsigned char **lines = calloc(MAXSTRS, sizeof(char*));
    unsigned char **lines_ptr = lines;
    for (int i = 0; i < MAXSTRS; ++i) {
        *lines_ptr = calloc(MAXSTRLEN, sizeof(char*));
        ++lines_ptr;
    }

    char *fin_name = calloc(MAXSTRLEN, sizeof(char*));
    strcpy(fin_name, "onegin.txt");
    char *fout_name = calloc(MAXSTRLEN, sizeof(char*));
    strcpy(fout_name, "oneginized.txt");
    if (argc > 1) {
        fin_name = argv[1];
    }
    if (argc > 2) {
        fout_name = argv[2];
    }

    int lines_cnt = read_lines(fin_name, lines);
    if (lines_cnt == ERROR) {
        free_memory(lines, fin_name, fout_name);
        return 0;
    }

    qsort(lines, lines_cnt, sizeof(char*), compare_strings);
    printf("%d lines are sorted!\n", lines_cnt);
    print_lines(fout_name, lines, lines_cnt);

    free_memory(lines, fin_name, fout_name);

    return 0;

}

char is_russian_letter(unsigned char c) {
    return (c >= (unsigned char) 'À' && c <= (unsigned char) 'ß') || (c >= (unsigned char) 'à' && c <= (unsigned char) 'ÿ');
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

int read_lines(char *file_name, unsigned char **lines) {
    FILE *fin = fopen(file_name, "r");
    if (!fin) {
        printf("File '%s' not found!\n", file_name);
        return ERROR;
    }

    unsigned char **lines_ptr = lines;
    int lines_cnt = 0;
    while (fgets(*(lines_ptr++), sizeof(char) * MAXSTRLEN, fin)) {
        lines_cnt += 1;
        if (lines_cnt == MAXSTRS - 1) {
            printf("Can't handle such a big file!\n");
            return ERROR;
        }
    }

    return lines_cnt;
}

void print_lines(char *file_name, unsigned char **lines, int lines_cnt) {
    FILE *fout = fopen(file_name, "w");
    for (int i = 0; i < lines_cnt; ++i) {
        fprintf(fout, "%s", lines[i]);
    }

    fclose(fout);
}
