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

    size_t prefill_size;
    std::optional<T> fill_elem;

    T *allocate_chunk(size_t from, size_t to) {
        T *chunk = (T*) calloc(CHUNK_SIZE, sizeof(T));
        
        if (!fill_elem.has_value()) {
            return chunk;
        }

        for (size_t i = from, idx = 0; i < std::min(prefill_size, to); ++i, ++idx) {
            chunk[idx] = fill_elem.value();
        }

        return chunk;
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
            printf("i: %ld\n", i);
            throw std::range_error("Bad index passed to data() of IndexedChunk storage");
        }

        if (data_.data(i)) {
            return data_.data(i);
        } else {
            return data_.data(i) = allocate_chunk(CHUNK_SIZE * i, CHUNK_SIZE * (i + 1));
        }
    }

public:
    constexpr static bool is_dynamic = true;

    IndexedChunk() :
        data_(),
        capacity_(0),
        size_(0),
        prefill_size(0),
        fill_elem()
    {}

    IndexedChunk(size_t length, const T &elem={}) :
        data_(length / CHUNK_SIZE + 1 * (!!((length % CHUNK_SIZE) != 0)), nullptr),
        capacity_(data_.size() * CHUNK_SIZE),
        size_(length),
        prefill_size(length),
        fill_elem(elem)
    {}

    ~IndexedChunk() {
        for (size_t chunk_i = 0; chunk_i * CHUNK_SIZE < capacity_; ++chunk_i) {
            if (!data_.data(chunk_i)) continue;

            for (size_t idx = 0; idx < CHUNK_SIZE; ++idx) {
                size_t i = chunk_i * CHUNK_SIZE + idx;
                if (i >= size_) break;

                data_.data(chunk_i)[idx].~T();
            }
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
