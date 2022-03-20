#pragma once

#include "indexed_dynamic.h"

#include <cmath>
#include <optional>



namespace kctf::storage
{


template <typename T>
class IndexedChunk {
    constexpr static size_t MIN_CAPACITY = 4;
    constexpr static size_t CHUNK_SIZE = 2; // exp2(log2(4096 / sizeof(T)) + 1);

    IndexedDynamic<T*> data_;

    size_t capacity_;
    size_t size_;

    size_t prefill_size_;
    std::optional<T> fill_elem_;

    T *allocate_chunk(size_t from, size_t to) {
        T *chunk = (T*) calloc(CHUNK_SIZE, sizeof(T));
        
        if (!fill_elem_.has_value()) {
            return chunk;
        }

        for (size_t i = from, idx = 0; i < std::min(prefill_size_, to); ++i, ++idx) {
            chunk[idx] = fill_elem_.value();
        }

        return chunk;
    }

    T *copy_chunk(T *chunk, size_t to_copy) {
        T *new_chunk = (T*) calloc(CHUNK_SIZE, sizeof(T));

        for (size_t i = 0; i < to_copy; ++i) {
            new(new_chunk + i) T(chunk[i]);
        }

        return new_chunk;
    }

    void free_chunk(size_t chunk_i) {
        if (!data_.data(chunk_i)) return;

        for (size_t idx = 0; idx < CHUNK_SIZE; ++idx) {
            size_t i = chunk_i * CHUNK_SIZE + idx;
            if (i >= size_) break;

            data_.data(chunk_i)[idx].~T();
        }
    }

    void grow_capacity(size_t capacity) {
        while (capacity_ < capacity) {
            grow_capacity();
        }
    }

    void grow_capacity() {
        size_t new_capacity = capacity_ + CHUNK_SIZE;
        T *new_chunk = allocate_chunk(capacity_, new_capacity);

        if (!new_chunk) {
            throw std::overflow_error("Can't allocate new chunk for IndexedChunk storage");
        }

        *data_.expand_one() = new_chunk;

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

    T *get_chunk(size_t i) {
        if (i >= data_.size()) {
            throw std::range_error("Bad index passed to data() of IndexedChunk storage");
        }

        if (data_.data(i)) {
            return data_.data(i);
        } else {
            return data_.data(i) = allocate_chunk(CHUNK_SIZE * i, CHUNK_SIZE * (i + 1));
        }
    }

    T *get_chunk(size_t i) const {
        if (i >= data_.size()) {
            throw std::range_error("Bad index passed to data() of IndexedChunk storage");
        }

        if (data_.data(i)) {
            return data_.data(i);
        } else {
            throw std::range_error("Bad index passed to data() of IndexedChunk storage");
        }
    }

public:
    constexpr static bool is_dynamic = true;

    IndexedChunk() :
        data_(),
        capacity_(0),
        size_(0),
        prefill_size_(0),
        fill_elem_()
    {}

    IndexedChunk(size_t length, const T &elem={}) :
        data_(length / CHUNK_SIZE + 1 * (!!((length % CHUNK_SIZE) != 0)), nullptr),
        capacity_(data_.size() * CHUNK_SIZE),
        size_(length),
        prefill_size_(length),
        fill_elem_(elem)
    {}

    IndexedChunk(const IndexedChunk &other) :
        data_(other.data_),
        capacity_(other.capacity_),
        size_(other.size_),
        prefill_size_(other.prefill_size_),
        fill_elem_(other.fill_elem_)
    {
        for (size_t chunk_i = 0; chunk_i < data_.size(); ++chunk_i) {
            if (data_.data(chunk_i)) {
                size_t to_copy = std::min(std::max((size_t) 0, CHUNK_SIZE * chunk_i - size_), CHUNK_SIZE);
                data_.data(chunk_i) = copy_chunk(data_.data(chunk_i), to_copy);
            }
        }
    }

    ~IndexedChunk() {
        for (size_t chunk_i = 0; chunk_i * CHUNK_SIZE < capacity_; ++chunk_i) {
            free_chunk(chunk_i);
        }

        capacity_ = 0;
        size_ = 0;
    }

    T &data(size_t i) {
        if (size_ <= i) {
            throw std::range_error("Bad index passed to data() of IndexedChunk storage");
        }

        return get_chunk(i / CHUNK_SIZE)[i % CHUNK_SIZE];
    }

    const T &data(size_t i) const {
        if (size_ <= i) {
            throw std::range_error("Bad index passed to data() of IndexedChunk storage");
        }

        return get_chunk(i / CHUNK_SIZE)[i % CHUNK_SIZE];
    }

    T *expand_one() {
        int placement_i = increment_size() - 1;
        return &data(placement_i);
    }

    void extract_one() {
        data(size() - 1).~T();
        decrement_size();
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
        grow_capacity(capacity);
    }

    void shrink_to_fit() {
        while (data_.size() && (data_.size() - 1) * CHUNK_SIZE >= size()) {
            free_chunk(data_.size() - 1);
            capacity_ -= CHUNK_SIZE;
            data_.extract_one();
        }
    }

// ============================================================================ modifirers

    void clear() {
        for (size_t i = 0; i < data_.size(); ++i) {
            free_chunk(i);
        }

        data_.clear();

        capacity_ = 0;
        size_ = 0;
        prefill_size_ = 0;
        fill_elem_ = std::nullopt;
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


} // namespace kctf::storage
