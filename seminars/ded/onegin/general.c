
char is_russian_letter(unsigned char c) {
    return (c >= (unsigned char) 'À' && c <= (unsigned char) 'ß') || (c >= (unsigned char) 'à' && c <= (unsigned char) 'ÿ');
}

char is_letter(unsigned char c) {
    return isalpha(c) || is_russian_letter(c);
}

void swap_ptrs(void **first, void **second) {
    void *tmp = *second;
    *second = *first;
    *first = tmp;
}

void qqh_sort(void *arr, int elem_cnt, size_t elem_size, int (*comp)(void *elem1, void *elem2)) {
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

int compare_lines(const void **elem1, const void **elem2) {
    unsigned char* str1 = ((Line_t*)(*elem1))->string;
    unsigned char* str2 = ((Line_t*)(*elem2))->string;

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

int reverse_compare_lines(const void **elem1, const void **elem2) {
    return -compare_lines(elem1, elem2);
}
