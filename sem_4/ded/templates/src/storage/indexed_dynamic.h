#pragma once


#include "allocator/Allocator.h"


namespace kctf::storage
{


template <typename T, template <typename U>  typename Allocator = allocator::Simple>
class IndexedDynamicT {
    constexpr static size_t MIN_CAPACITY = 4;

    Allocator<T> allocator_;

    T *data_;

    size_t capacity_;
    size_t size_;

    void set_capacity(size_t new_capacity) {
        T *new_data = allocator_.allocate(new_capacity);

        if (!new_data) {
            throw std::overflow_error("Can't allocate growed buffer for IndexedDynamic storage");
        }

        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
            
            data_[i].~T();
        }

        allocator_.deallocate(data_, capacity_);

        data_ = new_data;
        capacity_ = new_capacity;
    }

    void grow_capacity(float coef=2) {
        if (!capacity_) {
            capacity_ = MIN_CAPACITY;
        }

        size_t new_capacity = capacity_ * coef;
        set_capacity(new_capacity);
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
    constexpr static bool can_give_data_ptr = true;
    constexpr static bool can_modify_size = true;
    constexpr static bool provides_iterators = true;

    IndexedDynamicT() :
        data_(nullptr),
        capacity_(0),
        size_(0)
    {}

    explicit IndexedDynamicT(size_t length, const T &elem={}) :
        data_(allocator_.allocate(length)),
        capacity_(length),
        size_(length)
    {
        if (!data_) throw std::bad_alloc();

        for (size_t i = 0; i < size_; ++i) {
            new(&data_[i]) T(elem);
        }
    }

    explicit IndexedDynamicT(std::initializer_list<T> list) :
        data_(allocator_.allocate(list.size())),
        capacity_(list.size()),
        size_(list.size())
    {
        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            new(&data_[i]) T(std::move(*it));
        }
    }

    IndexedDynamicT(const IndexedDynamicT &other) :
        data_(allocator_.allocate(other.capacity_)),
        capacity_(other.capacity_),
        size_(other.size_)
    {
        for (size_t i = 0; i < size_; ++i) {
            new(data_ + i) T(other.data(i));
        }
    }

    IndexedDynamicT &operator=(const IndexedDynamicT &other) {
        if (&other == this) return *this;

        clear();

        data_ = (T*) calloc(other.capacity_, sizeof(T));
        if (!data_) throw std::bad_alloc();

        capacity_ = other.capacity_;
        size_ = other.size_;

        for (size_t i = 0; i < size_; ++i) {
            new(data_ + i) T(other.data(i));
        }

        return *this;
    }

    IndexedDynamicT(IndexedDynamicT &&other) :
        data_(std::move(other.data_)),
        capacity_(std::move(other.capacity_)),
        size_(std::move(other.size_))
    {
        if (&other == this) return *this;

        other.clear();
    }

    IndexedDynamicT &operator=(IndexedDynamicT &&other) {
        if (&other == this) return *this;

        clear();

        data_     = std::move(other.data_);
        capacity_ = std::move(other.capacity_);
        size_     = std::move(other.size_);

        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;

        return *this;
    }

    ~IndexedDynamicT() {
        clear();
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

    inline size_t size() const {
        return size_;
    }
    
    inline size_t capacity() const {
        return capacity_;
    }

    inline bool full() const {
        return size_ == capacity_;
    }

    void reserve(size_t capacity) {
        if (capacity_ >= capacity) return;
        set_capacity(capacity);
    }

    void shrink_to_fit() {
        set_capacity(size());
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

// ============================================================================ modifirers

    T *expand_one() {
        int placement_i = increment_size() - 1;
        return &data(placement_i);
    }

    void extract_one() {
        data(size() - 1).~T();
        decrement_size();
    }

    void clear() {
        for (size_t i = 0; i < size(); ++i) {
            data_[i].~T();
        }

        allocator_.deallocate(data_, capacity_);
        data_ = nullptr;

        capacity_ = 0;
        size_ = 0;
    }

    void resize(size_t new_size, const T &fill_elem={}) {
        while (size() > new_size) {
            extract_one();
        }

        while (size() < new_size) {
            *expand_one() = fill_elem;
        }
    }
};


template <template <typename U>  typename Allocator = allocator::Simple>
struct IndexedDynamic {
    template <typename T>
    using type = IndexedDynamicT<T, Allocator>;
};


} // namespace kctf::storage
