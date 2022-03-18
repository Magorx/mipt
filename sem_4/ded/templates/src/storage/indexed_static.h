#pragma once


namespace kctf::storage
{


template <typename T, int Size>
class IndexedStaticT {
    T data_[Size];

public:
    constexpr static bool is_dynamic = false;

    IndexedStaticT(const T &elem={})
    {
        for (size_t i = 0; i < Size; ++i) {
            new(&data_[i]) T(elem);
        }
    }

    ~IndexedStaticT() {
        for (size_t i = 0; i < Size; ++i) {
            data_[i].~T();
        }
    }

    T &data(size_t i) {
        if (Size <= i) {
            throw std::range_error("Bad index passed to data() of IndexedDynamic storage");
        }

        return data_[i];
    }

    constexpr inline size_t size() const {
        return Size;
    }
};

template<int Size>
struct IndexedStatic {
    template <typename T>
    using type = IndexedStaticT<T, Size>;
};


} // namespace kctf::storage
