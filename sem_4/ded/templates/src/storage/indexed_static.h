#pragma once


namespace kctf::storage
{


template <typename T, int Size>
class IndexedStaticT {
    T data_[Size];

public:
    constexpr static bool is_dynamic = false;
    constexpr static bool can_give_data_ptr = true;
    constexpr static bool can_modify_size = false;
    constexpr static bool provides_iterators = true;

    IndexedStaticT()
    {
        for (size_t i = 0; i < Size; ++i) {
            new(&data_[i]) T();
        }
    }

    explicit IndexedStaticT(const T &elem)
    {
        for (size_t i = 0; i < Size; ++i) {
            new(&data_[i]) T(elem);
        }
    }

    explicit IndexedStaticT(std::initializer_list<T> list) {
        if (list.size() > size()) {
            throw std::range_error("initializer_list of size " + std::to_string(list.size()) + "is too long to be transfered to StaticStorage of size " + std::to_string(size()));
        }

        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            new(&data_[i]) T(std::move(*it));
        }

        for (size_t i = list.size(); i < size(); ++i) {
            new(&data_[i]) T{};
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
        return data_[i];
    }

    const T &data(size_t i) const {
        return data_[i];
    }

// ============================================================================ capacity

    constexpr inline size_t size() const {
        return Size;
    }

    inline size_t capacity() const {
        return Size;
    }

// ============================================================================ iterators

    T *begin() {
        return data_;
    }

    T *end() {
        return data_ + size();
    }

    T *begin() const {
        return data_;
    }

    T *end() const {
        return data_ + size();
    }
};

template<int Size>
struct IndexedStatic {
    template <typename T>
    using type = IndexedStaticT<T, Size>;
};


} // namespace kctf::storage
