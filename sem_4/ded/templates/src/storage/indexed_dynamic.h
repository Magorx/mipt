#pragma once


namespace kctf::storage
{


template <typename T>
class IndexedDynamic {
    constexpr static size_t MIN_CAPACITY = 4;

    T *data_;

    size_t capacity_;
    size_t size_;

    void grow_capacity(float coef=2) {
        if (!capacity_) {
            capacity_ = MIN_CAPACITY;
        }

        size_t new_capacity = capacity_ * coef;
        T *new_data = (T*) calloc(new_capacity, sizeof(T));

        if (!new_data) {
            throw std::overflow_error("Can't allocate growed buffer for IndexedDynamic storage");
        }

        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
            
            data_[i].~T();
        }

        free(data_);

        data_ = new_data;
        capacity_ = new_capacity;
    }

    int increment_size() {
        if (full()) {
            grow_capacity();
        }

        return ++size_;
    }

    int decrement_size() {
        return --size_;
    }

public:
    constexpr static bool is_dynamic = true;

    IndexedDynamic() :
        data_(nullptr),
        capacity_(0),
        size_(0)
    {}

    IndexedDynamic(size_t length, const T &elem={}) :
        data_((T*) calloc(length, sizeof(T))),
        capacity_(length),
        size_(length)
    {
        if (!data_) throw std::overflow_error("Can't allocate buffer for IndexedDynamic storage");

        for (size_t i = 0; i < size_; ++i) {
            new(&data_[i]) T(elem);
        }
    }

    ~IndexedDynamic() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }

        free(data_);
        capacity_ = 0;
        size_ = 0;
    }

    T &data(size_t i) {
        if (size_ <= i) {
            throw std::range_error("Bad index passed to data() of IndexedDynamic storage");
        }

        return data_[i];
    }

    T *expand_one() {
        int placement_i = increment_size() - 1;
        return &data(placement_i);
    }

    void extract_one() {
        data(size() - 1).~T();
        decrement_size();
    }

    inline size_t size() const {
        return size_;
    }

    inline bool full() const {
        return size_ == capacity_;
    }
};


} // namespace kctf::storage
