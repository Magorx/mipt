char __costyle__is_russian_letter(const unsigned char c) {
    return (c >= (unsigned char) 'À' && c <= (unsigned char) 'ß') || (c >= (unsigned char) 'à' && c <= (unsigned char) 'ÿ');
}

char is_letter(const unsigned char c) {
    return isalpha(c);
}

void swap_ptrs(void **first, void **second) {
    void *tmp = *second;
    *second = *first;
    *first = tmp;
}

void qqh_sort(void *arr, const size_t elem_cnt, const size_t elem_size, int (*comp)(void *elem1, void *elem2)) {
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

int compare_lines_letters(const void *elem1, const void *elem2) {
    const unsigned char* str1 = (**(Line_t**)elem1).string;
    const unsigned char* str2 = (**(Line_t**)elem2).string;

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

    file->file_dscr = open(file->name, O_BINARY);
    if (!file->file_dscr) {
        return ERROR_FILE_NOT_FOUND;
    }

    read(file->file_dscr, file->text, file->info.st_size);
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
