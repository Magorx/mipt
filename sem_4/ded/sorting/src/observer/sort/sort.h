#pragma once


#include "observer/observed.h"
#include "utils/vec2d.h"


template<typename T>
struct SortStatistics {
    int asgn_cnt;
    int comp_cnt;

    size_t elements_cnt;

    SortStatistics() : asgn_cnt(0), comp_cnt(0) {}

    enum class ResultType {
        asgn,
        comp
    };

    int get_result(ResultType type) {
        switch (type)
        {
        case ResultType::asgn:
            return asgn_cnt;
        case ResultType::comp:
            return comp_cnt;
        
        default:
            return 0;
        }
    }

    bool is_asgn(Operator op) {
        return op == Operator::asgn || op == Operator::ctor_copy || op == Operator::asgn_copy || op == Operator::ctor || op == asgn_move || op == ctor_move;
    }

    bool is_comp(Operator op) {
        return  00
                || op == Operator::eq
                || op == Operator::ne
                || op == Operator::lt
                || op == Operator::le
                || op == Operator::gt
                || op == Operator::ge
                ||
                00;
    }

    void signal_record(const OperatorSignal<int> &sig) {
        if (is_asgn(sig.op)) {
            asgn_cnt++;
        }

        if (is_comp(sig.op)) {
            comp_cnt++;
        }
    }

    void bind(ObservedPool<T> &pool) {
        pool.push_observer([this](const OperatorSignal<int> &sig){ signal_record(sig); });
    }
};


template< class RandomIt >
using sort_func_signature = void (*)(RandomIt first, RandomIt last);


template<typename T>
SortStatistics<T> gather_sort_statistics(std::vector<Observed<T>> test, sort_func_signature<Observed<T>*> sort_func) {
    if (!test.size() || !sort_func) {
        return {};
    }

    SortStatistics<T> stats;
    auto &pool = test[0].get_pool();

    stats.bind(pool);
    stats.elements_cnt = test.size();

    pool.set_active(true);
    sort_func(&test[0], &test[0] + test.size());
    
    pool.pop_observer();
    return stats;
}


template<typename T>
T randlong() {
    T ret = rand();
    ret = ret | (T)(rand() << sizeof(int));
    return ret;
}


template<typename T>
std::vector<Observed<T>> generate_test_data(size_t size, Observed<T> (*func)(int x) = nullptr) {
    std::vector<Observed<T>> arr(size);
    for (size_t i = 0; i < size; ++i) {
        arr[i] = func ? func(randlong<int>()) : randlong<int>();
    }

    return arr;
}


template<typename T>
std::vector<std::vector<Observed<T>>> *generate_tests(  const double step = 25,
                                                        double mult = 1,
                                                        const int init_size = 10,
                                                        const int test_number = 100) {
    double cur_size = init_size;
    std::vector<std::vector<Observed<T>>> *ret = new std::vector<std::vector<Observed<T>>>;

    for (int i = 0; i < test_number; ++i) {
        ret->push_back(generate_test_data<T>((size_t) cur_size));

        cur_size = (cur_size + step) * mult;
    }

    return ret;
}


template<typename T>
std::vector<Vec2d> get_graph(std::vector<SortStatistics<T>> stats, typename SortStatistics<T>::ResultType type) {
    std::vector<Vec2d> ret(stats.size() + 1, {0, 0});
    for (size_t i = 0; i < stats.size(); ++i) {
        ret[i + 1] = {(double) stats[i].elements_cnt, (double) stats[i].get_result(type)};
    }

    return ret;
}


template <typename RandomAccessIterator>
void bubble_sort(RandomAccessIterator begin, RandomAccessIterator end) {
  bool swapped = true;

  while (begin != end-- && swapped) {
    swapped = false;

    for (auto i = begin; i != end; ++i) {
      if (*(i + 1) < *i) {
        std::iter_swap(i, i + 1);
        swapped = true;
      }
    }
  }
}


template<typename Iterator, typename Comparator>
void selection_sort(Iterator begin, Iterator end, Comparator comparator) {
    for (; begin != end; ++begin) {

        auto min = std::min_element(begin, end, comparator);

        if (comparator(*min, *begin)) {
            std::iter_swap(min, begin);
        }
    }
}

template<typename Iterator>
void selection_sort(Iterator begin, Iterator end) {
    typedef typename std::iterator_traits<Iterator>::value_type value_type;

    selection_sort(begin, end, std::less<value_type>());
}


template<typename T>
void merge_sort(T *begin, T *end) {
    size_t size = (end - begin) / sizeof(*begin);

    if (size <= 1) return;

    T *left = begin;
    T *middle = left + size / 2;
    T *right = end;

    merge_sort(left, middle);
    merge_sort(middle, right);
    std::inplace_merge(left, middle, right);
}

template<typename BdIterator>
void insertions_sort(BdIterator beg, BdIterator end)
{
    auto cmp = std::less<typename std::iterator_traits<BdIterator>::value_type>();
    BdIterator front, cur, prev;
    for (front = beg + 1u; front != end; ++front)
    {
        cur = front;
        prev = cur - 1u;
        while (cur != beg && cmp(*cur, *prev))
            std::iter_swap(cur--, prev--);
    }
}