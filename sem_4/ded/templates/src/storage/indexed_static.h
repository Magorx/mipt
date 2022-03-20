#pragma once


namespace kctf::storage
{


template <typename T, int Size>
class IndexedStaticT {
    T data_[Size];

public:
    constexpr static bool is_dynamic = false;
    constexpr static bool can_give_data_ptr = true;

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

    IndexedStaticT(const IndexedStaticT<T, Size> &other)
    {
        for (size_t i = 0; i < Size; ++i) {
            new(&data_[i]) T(other.data_[i]);
        }
    }

    IndexedStaticT &operator=(const IndexedStaticT<T, Size> &other)
    {
        for (size_t i = 0; i < Size; ++i) {
            new(&data_[i]) T(other.data_[i]);
        }

        return *this;
    }

    IndexedStaticT(IndexedStaticT<T, Size> &&other)
    {
        for (size_t i = 0; i < Size; ++i) {
            data_[i] = std::move(other.data_[i]);
        }
    }

    IndexedStaticT &operator=(IndexedStaticT<T, Size> &&other)
    {
        for (size_t i = 0; i < Size; ++i) {
            data_[i] = std::move(other.data_[i]);
        }

        return *this;
    }

    T *data_ptr() {
        return data_;
    }

    const T *data_ptr() const {
        return data_;
    }

    T &data(size_t i) {
        if (Size <= i) {
            throw std::range_error("Bad index passed to data() of IndexedDynamic storage");
        }

        return data_[i];
    }

    const T &data(size_t i) const {
        if (Size <= i) {
            throw std::range_error("Bad index passed to data() of IndexedDynamic storage");
        }

        return data_[i];
    }

// ============================================================================ capacity

    constexpr inline size_t size() const {
        return Size;
    }

    inline size_t capacity() const {
        return Size;
    }
};

template<int Size>
struct IndexedStatic {
    template <typename T>
    using type = IndexedStaticT<T, Size>;
};


} // namespace kctf::storage
