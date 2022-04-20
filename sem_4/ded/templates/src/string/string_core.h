#pragma once


#include "allocator/Allocator.h"


namespace kctf
{


template <typename CharT, template <typename U>  typename Allocator = allocator::Simple>
class StringCore {
private:
    constexpr static size_t CharSize = sizeof(CharT);
    constexpr static size_t MIN_CAPACITY = 4;

    Allocator<CharT> allocator_;

    CharT *data_;

    size_t capacity_;
    size_t size_;

    void set_capacity(size_t new_capacity) {
        CharT *new_data = allocator_.allocate(new_capacity);

        if (!new_data) {
            throw std::overflow_error("Can't allocate growed buffer for StringCore");
        }

        if (data_) {
            memcpy(new_data, data_, capacity_ * CharSize);
            allocator_.deallocate(data_, capacity_);
        }

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

protected:
    StringCore() :
        data_(nullptr),
        capacity_(0),
        size_(0)
    {}

    explicit StringCore(size_t length, const CharT &elem={}) :
        data_(allocator_.allocate(length)),
        capacity_(length),
        size_(length)
    {
        if (!data_) throw std::bad_alloc();

        for (size_t i = 0; i < size_; ++i) {
            new(&data_[i]) CharT(elem);
        }
    }

    explicit StringCore(std::initializer_list<CharT> list) :
        data_(allocator_.allocate(list.size())),
        capacity_(list.size()),
        size_(list.size())
    {
        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            new(&data_[i]) CharT(std::move(*it));
        }
    }

    StringCore(const StringCore &other) :
        data_(allocator_.allocate(other.capacity_)),
        capacity_(other.capacity_),
        size_(other.size_)
    {
        for (size_t i = 0; i < size_; ++i) {
            new(data_ + i) CharT(other.data_at(i));
        }
    }

    StringCore &operator=(const StringCore &other) {
        if (&other == this) return *this;

        clear();

        data_ = (CharT*) calloc(other.capacity_, sizeof(CharT));
        if (!data_) throw std::bad_alloc();

        capacity_ = other.capacity_;
        size_ = other.size_;

        for (size_t i = 0; i < size_; ++i) {
            new(data_ + i) CharT(other.data_at(i));
        }

        return *this;
    }

    StringCore(StringCore &&other) :
        data_(std::move(other.data_)),
        capacity_(std::move(other.capacity_)),
        size_(std::move(other.size_))
    {
        if (&other == this) return *this;

        other.clear();
    }

    StringCore &operator=(StringCore &&other) {
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

    ~StringCore() {
        clear();
    }

    CharT *data_ptr() {
        return data_;
    }

    const CharT *data_ptr() const {
        return data_;
    }

    CharT &data_at(size_t i) {
        return data_[i];
    }

    const CharT &data_at(size_t i) const {
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

    CharT *begin() {
        return data_;
    }

    CharT *end() {
        return data_ + size();
    }

    CharT *begin() const {
        return data_;
    }

    CharT *end() const {
        return data_ + size();
    }

// ============================================================================ modifirers

    inline CharT *expand_one() {
        int placement_i = increment_size() - 1;
        return &data_at(placement_i);
    }

    inline void extract_one() {
        data_at(size() - 1).~CharT();
        decrement_size();
    }

    void clear() {
        for (size_t i = 0; i < size(); ++i) {
            data_[i].~CharT();
        }

        allocator_.deallocate(data_, capacity_);
        data_ = nullptr;

        capacity_ = 0;
        size_ = 0;
    }

    void resize(size_t new_size, const CharT &fill_elem={}) {
        while (size() > new_size) {
            extract_one();
        }

        while (size() < new_size) {
            *expand_one() = fill_elem;
        }
    }

    void swap(StringCore<CharT, Allocator> &other) {
        std::swap(allocator_, other.allocator_);
        std::swap(data_, other.data_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }
};

} // namespace kctf::storage
