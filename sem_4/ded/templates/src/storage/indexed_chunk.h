#pragma once

#include "indexed_dynamic.h"

#include <cmath>
#include <optional>



namespace kctf::storage
{


template <typename T, int ChunkSize, template <typename U>  typename Allocator = allocator::Simple>
class IndexedChunkedT {
    constexpr static size_t MIN_CAPACITY = 4;
    
    Allocator<T> allocator_;

    IndexedDynamicT<T*, Allocator> data_;

    size_t capacity_;
    size_t size_;

    size_t prefill_size_;
    std::optional<T> fill_elem_;

    T *allocate_chunk(size_t from, size_t to) {
        T *chunk = allocator_.allocate(ChunkSize); // (T*) new char[ChunkSize * sizeof(T)];
        
        if (!fill_elem_.has_value()) {
            return chunk;
        }

        for (size_t i = from, idx = 0; i < std::min(prefill_size_, to); ++i, ++idx) {
            chunk[idx] = fill_elem_.value();
        }

        return chunk;
    }

    T *copy_chunk(T *chunk, size_t to_copy) {
        if (!chunk) return nullptr;

        T *new_chunk = allocator_.allocate(ChunkSize); // (T*) new char[ChunkSize * sizeof(T)];

        for (size_t i = 0; i < to_copy; ++i) {
            new(new_chunk + i) T(chunk[i]);
        }

        return new_chunk;
    }

    void free_chunk(size_t chunk_i) {
        if (!data_.data(chunk_i)) return;

        for (size_t idx = 0; idx < ChunkSize; ++idx) {
            size_t i = chunk_i * ChunkSize + idx;
            if (i >= size_) break;

            data_.data(chunk_i)[idx].~T();
        }

        allocator_.deallocate(data_.data(chunk_i), ChunkSize); // delete[] data_.data(chunk_i);
    }

    void grow_capacity(size_t capacity) {
        while (capacity_ < capacity) {
            grow_capacity();
        }
    }

    void grow_capacity() {
        size_t new_capacity = capacity_ + ChunkSize;
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
            return data_.data(i) = allocate_chunk(ChunkSize * i, ChunkSize * (i + 1));
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
    constexpr static bool can_give_data_ptr = false;
    constexpr static bool can_modify_size = true;
    constexpr static bool provides_iterators = false;

    IndexedChunkedT() :
        data_(),
        capacity_(0),
        size_(0),
        prefill_size_(0),
        fill_elem_()
    {}

    explicit IndexedChunkedT(size_t length, const T &elem={}) :
        data_(length / ChunkSize + 1 * (!!((length % ChunkSize) != 0)), nullptr),
        capacity_(data_.size() * ChunkSize),
        size_(length),
        prefill_size_(length),
        fill_elem_(elem)
    {}

    explicit IndexedChunkedT(std::initializer_list<T> list) :
        data_(),
        capacity_(0),
        size_(0)
    {
        for (auto it = list.begin(); it != list.end(); ++it) {
            new(expand_one()) T(std::move(*it));
        }
    }

    IndexedChunkedT(const IndexedChunkedT &other) :
        data_(other.data_),
        capacity_(other.capacity_),
        size_(other.size_),
        prefill_size_(other.prefill_size_),
        fill_elem_(other.fill_elem_)
    {
        for (size_t chunk_i = 0; chunk_i < data_.size(); ++chunk_i) {
            if (data_.data(chunk_i)) {
                size_t to_copy = std::min(std::max((size_t) 0, ChunkSize * chunk_i - size_), (size_t) ChunkSize);
                data_.data(chunk_i) = copy_chunk(data_.data(chunk_i), to_copy);
            }
        }
    }

    IndexedChunkedT &operator=(const IndexedChunkedT &other) {
        if (&other == this) return *this;

        clear();

        data_ = other.data_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        prefill_size_ = other.prefill_size_;
        fill_elem_ = other.fill_elem_;

        for (size_t chunk_i = 0; chunk_i < data_.size(); ++chunk_i) {
            if (data_.data(chunk_i)) {
                size_t to_copy = std::min(std::max((size_t) 0, ChunkSize * chunk_i - size_), (size_t) ChunkSize);
                data_.data(chunk_i) = copy_chunk(data_.data(chunk_i), to_copy);
            }
        }

        return *this;
    }

    IndexedChunkedT(IndexedChunkedT &&other) :
        data_(std::move(other.data_)),
        capacity_(std::move(other.capacity_)),
        size_(std::move(other.size_)),
        prefill_size_(std::move(other.prefill_size_)),
        fill_elem_(std::move(other.fill_elem_))
    {
        other.clear();
    }

    IndexedChunkedT &operator=(IndexedChunkedT &&other) {
        if (&other == this) return *this;

        clear();

        data_ = std::move(other.data_);
        capacity_ = std::move(other.capacity_);
        size_ = std::move(other.size_);
        prefill_size_ = std::move(other.prefill_size_);
        fill_elem_ = std::move(other.fill_elem_);

        other.clear();

        return *this;
    }

    ~IndexedChunkedT() {
        clear();
    }

    T &data(size_t i) {
        return get_chunk(i / ChunkSize)[i % ChunkSize];
    }

    const T &data(size_t i) const {
        return get_chunk(i / ChunkSize)[i % ChunkSize];
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
        while (data_.size() && (data_.size() - 1) * ChunkSize >= size()) {
            free_chunk(data_.size() - 1);
            capacity_ -= ChunkSize;
            data_.extract_one();
        }
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


template <int ChunkSize, template <typename U>  typename Allocator = allocator::Simple>
struct IndexedChunked {
    template <typename T>
    using type = IndexedChunkedT<T, ChunkSize, Allocator>;
};


} // namespace kctf::storage
