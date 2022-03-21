#pragma once


#include "storage/Indexed.h"


namespace kctf {


template <
          typename T,
          template <typename U> typename IndexedStorageT
         >
class ArrayT {
protected:
    IndexedStorageT<T> storage_;

    // template <typename PushBackT>
    // constexpr void validate_data_type(PushBackT &&) {
    //     static_assert(std::is_same_v<std::remove_cvref_t<PushBackT>, T>);
    // }
    
public:
    ArrayT() : storage_() {
    }

    ArrayT(size_t length, const T &elem={}) : storage_(length, elem) {
    }

    ArrayT(const ArrayT &other) :
    storage_(other.storage_) {
    }

    ArrayT &operator=(const ArrayT &other) {
        storage_ = other.storage_;
        return *this;
    }

    ArrayT(ArrayT &&other) :
    storage_(std::move(other.storage_)) {
    }

    ArrayT &operator=(ArrayT &&other) {
        storage_ = std::move(other.storage_);
        return *this;
    }

// ============================================================================ element access

    T &operator[](size_t i) {
        return storage_.data(i);
    }

    const T &operator[](size_t i) const {
        return storage_.data(i);
    }

    T &at(size_t i) {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return storage_.data(i);
    }

    const T &at(size_t i) const {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return storage_.data(i);
    }

    T &front() {
        return (*this)[0];
    }

    const T &front() const {
        return (*this)[0];
    }

    T &back() {
        return (*this)[size() - 1];
    }

    const T &back() const {
        return (*this)[size() - 1];
    }

    T &at_front() {
        if (empty()) throw std::range_error("can't take front() from ArrayT of size 0");

        return (*this)[0];
    }

    const T &at_front() const {
        if (empty()) throw std::range_error("can't take front() from ArrayT of size 0");

        return (*this)[0];
    }

    T &at_back() {
        if (empty()) throw std::range_error("can't take back() from ArrayT of size 0");

        return (*this)[size() - 1];
    }

    const T &at_back() const {
        if (empty()) throw std::range_error("can't take back() from ArrayT of size 0");

        return (*this)[size() - 1];
    }

    T *data() {
        static_assert(IndexedStorageT<T>::can_give_data_ptr);
        return storage_.data_ptr();
    }

    const T *data() const {
        static_assert(IndexedStorageT<T>::can_give_data_ptr);
        return storage_.data_ptr();
    }

// ============================================================================ capacity

    constexpr inline size_t size() const {
        return storage_.size();
    }

    constexpr inline bool empty() const {
        return !!storage_.size();
    }

    void reserve(size_t capacity) {
        storage_.reserve(capacity);
    }

    size_t capacity() const {
        return storage_.capacity();
    }

    void shrink_to_fit() {
        storage_.shrink_to_fit();
    }

// ============================================================================ modifiers

    template <typename Q>
    void push_back(Q &&elem) {
        static_assert(IndexedStorageT<T>::can_modify_size);

        T *placement = storage_.expand_one();
        new(placement) T(std::forward<Q>(elem));
    }

    template <typename ... Ts>
    void emplace_back(Ts ... vs) {
        static_assert(IndexedStorageT<T>::can_modify_size);

        T *placement = storage_.expand_one();
        new(placement) T(std::forward<Ts>(vs)...);
    }

    void pop_back() {
        static_assert(IndexedStorageT<T>::can_modify_size);

        storage_.extract_one();
    }

    void clear() {
        static_assert(IndexedStorageT<T>::can_modify_size);

        storage_.clear();
    }

    void resize(size_t new_size, const T &fill_elem={}) {
        static_assert(IndexedStorageT<T>::can_modify_size);

        storage_.resize(new_size, fill_elem);
    }

    void fill(const T &elem) {
        for (size_t i = 0; i < size(); ++i) {
            storage_.data(i) = elem;
        }
    }
};


struct BoolProxy {
    uint8_t &byte_;
    uint8_t  mask_;
    uint8_t  idx_;

    BoolProxy(const BoolProxy &other) = delete;
    BoolProxy& operator=(const BoolProxy &other) = delete;

    BoolProxy(uint8_t &byte, uint8_t index) :
        byte_(byte),
        mask_(1 << index),
        idx_(index)
    {}

    BoolProxy& operator=(bool value) {
        byte_ &= ~mask_;
        byte_ |= ((uint8_t) value) << idx_;
        return *this;
    }

    void set_value(bool value) {
        byte_ &= ~mask_;
        byte_ |= ((uint8_t) value) << idx_;
    }

    operator bool() {
        return byte_ & mask_;
    }
};


template <
          template <typename U> typename IndexedStorageT
         >
class ArrayT<bool, IndexedStorageT> : private ArrayT<uint8_t, IndexedStorageT> {
    size_t bools_cnt; 

    using BaseArray = ArrayT<uint8_t, IndexedStorageT>;
public:
    BoolProxy operator[](size_t i) {
        return BoolProxy(BaseArray::storage_.data(i / 8), i % 8);
    }

    ArrayT() :
        ArrayT<uint8_t, IndexedStorageT>(),
        bools_cnt(IndexedStorageT<uint8_t>::is_dynamic ? 0 : BaseArray::storage_.size())
    {}

    ArrayT(size_t length, const bool elem={}) :
        ArrayT<uint8_t, IndexedStorageT>(length / 8 + 1 * (!!(length % 8 != 0)), elem),
        bools_cnt(length)
    {}

    void push_back(bool elem) {
        if (bools_cnt % 8 == 0) {
            uint8_t *placement = BaseArray::storage_.expand_one();
            BoolProxy proxy(*placement, 0);
            proxy = elem;
        } else {
            (*this)[bools_cnt] = elem;
        }
        ++bools_cnt;
    }

    void emplace_back(bool elem) {
        push_back(elem);
    }

    void pop_back() {
        --bools_cnt;
        if (bools_cnt % 8 == 0) {
            BaseArray::storage_.extract_one();
        }
    }

    constexpr inline size_t size() const {
        return bools_cnt;
    }

    constexpr inline bool empty() const {
        return !!bools_cnt;
    }

    void clear() {
        size_t cur_size = size();
        for (size_t i = 0; i < cur_size; ++i) {
            (*this)[i].~T();
        }
    }
};


template <typename T>
using Vector = ArrayT<T, storage::IndexedDynamic>;

template <typename T, int Size>
using Array = ArrayT<T, storage::IndexedStatic<Size>::template type>;

template <typename T, int ChunkSize>
using ChunkVector = ArrayT<T, storage::IndexedChunked<ChunkSize>::template type>;

} // namespace kctf
