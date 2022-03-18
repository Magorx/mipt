#pragma once


#include <type_traits>


template <typename T, int Size>
struct Vec {
    T data_[Size];

    template <typename ... Ts>
    Vec(Ts ... vs) :
    data_{std::forward<Ts>(vs)...}
    {
        static_assert((std::is_convertible_v<Ts, T> && ...) && sizeof...(Ts) <= Size);
    }

    T dot(const Vec<T, Size> &other) {
        return dot<Size>(data_, other.data_);
    }

    T &operator[](size_t i) {
        return data_[i];
    }

private:
    template <int SizeLeft>
    static T dot(const T *first, const T *second) {
        (void) first;
        (void) second;

        if constexpr (SizeLeft == 0) {
            return T{};
        } else if constexpr (SizeLeft == 1) {
            return (*first) * (*second);
        } else {
            return (*first) * (*second) + dot<SizeLeft - 1>(first + 1, second + 1);
        }
    }
};
