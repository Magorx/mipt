#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
 
void count_sort(const std::vector<char> &string, std::vector<int> &sortable)
{
    std::vector<int> counter(255, 0);
    size_t string_len = string.size();
 
    for (int i = 0; i < string_len; ++i)
    {
        counter[string[i]]++;
    }
 
    for (int i = 1; i < 255; ++i)
    {
        counter[i] += counter[i - 1];
    }
 
    for (int i = string_len - 1; i >= 0; --i)
    {
        sortable[--counter[string[i]]] = i;
    }
}

void print_vec(const std::vector<int> &vec, const int delta = 1) {
    for (auto x : vec) {
        std::cout << x + delta << " ";
    }
    std::cout << "\n";
}
 
void count_suffix_array(const std::vector<char> &string, std::vector<int> &suffix_array)
{
    int string_len = string.size();
 
    std::vector<int> counter(string_len, 0);
    std::vector<int> suffix_array_tmp(string_len, 0);
    std::vector<int> equivalence_class_number(string_len, 0);
 
    // нулевая итерация построения суффиксного массива
    count_sort(string, suffix_array_tmp);
 
    int current_equivalence_class = 1;
    equivalence_class_number[suffix_array_tmp[0]] = 0;
    for (int i = 1; i < string_len; ++i)
    {
        if (string[suffix_array_tmp[i]] == string[suffix_array_tmp[i - 1]])
            equivalence_class_number[suffix_array_tmp[i]] = current_equivalence_class - 1;
        else
        {
            equivalence_class_number[suffix_array_tmp[i]] = current_equivalence_class;
            current_equivalence_class++;
        }
    }
 
    // остальные итерации алгоритма построения суффиксного массива
    for (int i = 1; i < string_len; i *= 2)
    {
		print_vec(suffix_array_tmp);
        std::vector<int> second_class_order(string_len, 0);
        std::vector<int> tmp_equivalence_class_number(string_len, 0);
 
        for (int j = 0; j < string_len; ++j)
        {
            if (suffix_array_tmp[j] >= i)
                second_class_order[j] = suffix_array_tmp[j] - i;
            else
                second_class_order[j] = suffix_array_tmp[j] - i + string_len;
        }
        
        for (int k = 0; k < current_equivalence_class; ++k)
        {
            counter[k] = 0;
        }
 
        // пародия на сортировку подсчётом
        for (int k = 0; k < string_len; ++k)
        {
            counter[equivalence_class_number[second_class_order[k]]]++;
        }
        for (int k = 1; k < current_equivalence_class; ++k)
        {
            counter[k] += counter[k - 1];
        }
        for (int k = string_len - 1; k >= 0; --k)
        {
            suffix_array_tmp[--counter[equivalence_class_number[second_class_order[k]]]] = second_class_order[k];
        }
 
        int tmp_equivalence_class = 1;
        tmp_equivalence_class_number[suffix_array_tmp[0]] = 0;
        
        for (int k = 1; k < string_len; ++k)
        {
            int right_middle = (suffix_array_tmp[k] + i) % string_len;
            int left_middle = (suffix_array_tmp[k - 1] + i) % string_len;
            
            if (equivalence_class_number[suffix_array_tmp[k]] != equivalence_class_number[suffix_array_tmp[k - 1]] ||
                equivalence_class_number[right_middle] != equivalence_class_number[left_middle])
                tmp_equivalence_class++;
 
            tmp_equivalence_class_number[suffix_array_tmp[k]] = tmp_equivalence_class - 1;
        }
 
        for (int k = 0; k < string_len; ++k)
        {
            equivalence_class_number[k] = tmp_equivalence_class_number[k];
        }
 
        current_equivalence_class = tmp_equivalence_class;
    }
 
    for (int k = 0; k < string_len; ++k)
    {
        suffix_array[k] = suffix_array_tmp[k];
    }
}
 
int main()
{
    std::vector<char> string;
    char symbol = 0;
    while (scanf("%c", &symbol) && symbol != '\n')
    {
        string.push_back(symbol);
    }
    string.push_back(0);
    int string_len = string.size();
 
    std::vector<int> suffix_array(string_len, 0);
    count_suffix_array(string, suffix_array);
 
    for (int i = 1; i < string_len; ++i)
        printf("%d ", suffix_array[i] + 1);
    printf("\n");
 
    return 0;
}
 