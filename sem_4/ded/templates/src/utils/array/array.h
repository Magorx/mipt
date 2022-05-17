#pragma once


#include "storage/Indexed.h"
#include "allocator/Allocator.h"


namespace kctf {


template <typename Array, typename T>
class Iterator {
    Array *array_;
    size_t index_;

public:

    static constexpr bool is_const = std::is_same_v<T, const typename Array::value_type>;

    using value_type      = typename Array::value_type;
    using difference_type = typename Array::difference_type;
    
    using pointer   = std::conditional_t<is_const, typename Array::const_pointer,   typename Array::pointer>;
    using reference = std::conditional_t<is_const, typename Array::const_reference, typename Array::reference>;

    using iterator_category = std::random_access_iterator_tag;

    Iterator() : array_(nullptr), index_(0) {}

    Iterator(Array &array, size_t index)
        : array_(&array),
          index_(index) {
    }

    Iterator &operator++() {
        ++index_;
        
        assert(index_ <= array_->size());
        return *this;
    }

    Iterator operator++(int) {
        Iterator<Array, T> ret{array_, index_};
        ++index_;

        assert(index_ <= array_->size());
        return ret;
    }

    Iterator &operator--() {
        --index_;

        assert(index_ <= array_->size());
        return *this;
    }

    Iterator operator--(int) {
        Iterator<Array, T> ret{array_, index_};
        --index_;

        assert(index_ <= array_->size());
        return ret;
    }

    difference_type operator-(const Iterator &other) const {
        assert(array_ == other.array_);

        return index_ - other.index_;
    }

    Iterator operator+(difference_type diff) {
        Iterator ret{*array_, index_ + diff};

        assert(ret.index_ <= array_->size());
        return ret;
    }

    Iterator &operator+=(difference_type diff) {
        index_ += diff;

        assert(index_ <= array_->size());
        return *this;
    }

    Iterator operator-(difference_type diff) {
        Iterator ret{*array_, index_ - diff};

        assert(ret.index_ <= array_->size());
        return ret;
    }

    Iterator &operator-=(difference_type diff) {
        index_ -= diff;

        assert(index_ <= array_->size());
        return *this;
    }

    T &operator[](difference_type diff) {
        return (*array_)[index_ + diff];
    }
    
    T &operator[](difference_type diff) const {
        return (*array_)[index_ + diff];
    }

    bool operator!=(const Iterator &other) const {
        assert(array_ == other.array_);
        return index_ != other.index_;
    }
    
    bool operator==(const Iterator &other) const {
        assert(array_ == other.array_);
        return index_ == other.index_;
    }

    bool operator<(const Iterator &other) const {
        assert(array_ == other.array_);
        return index_ < other.index_;
    }

    bool operator>(const Iterator &other) const {
        assert(array_ == other.array_);
        return index_ > other.index_;
    }

    bool operator<=(const Iterator &other) const {
        assert(array_ == other.array_);
        return index_ < other.index_;
    }

    bool operator>=(const Iterator &other) const {
        assert(array_ == other.array_);
        return index_ > other.index_;
    }

    T &operator*() {
        return (*array_)[index_];
    }

    T *operator->() {
        return &(*array_)[index_];
    }
};


template <
          typename T,
          template <typename U> typename IndexedStorageT
         >
class ArrayT {
protected:
    IndexedStorageT<T> storage_;

    using ArrayType = ArrayT<T, IndexedStorageT>;
    using IndexedStorageType = IndexedStorageT<T>;

    // template <typename PushBackT>
    // constexpr void validate_data_type(PushBackT &&) {
    //     static_assert(std::is_same_v<std::remove_cvref_t<PushBackT>, T>);
    // }
    
public:

    using value_type = T;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    ArrayT() : storage_() {
    }

    explicit ArrayT(size_t length, const T &elem={}) : storage_(length, elem) {
    }

    explicit ArrayT(std::initializer_list<T> list) : storage_(list) {
    }

    // TODO .copy(...);
    // explicit ArrayT(const ArrayT &other) :
    // storage_(other.storage_) {
    // }

    // ArrayT &operator=(const ArrayT &other) {
    //     storage_ = other.storage_;
    //     return *this;
    // }

    ArrayT(ArrayT &&other_move) :
    storage_(std::move(other_move.storage_)) {
    }

    ArrayT &operator=(ArrayT &&other_move) {
        if (&other_move == this) return *this;

        storage_ = std::move(other_move.storage_);
        return *this;
    }

// ============================================================================ element access

    [[nodiscard]] inline T &operator[](size_t i) {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return storage_.data(i);
    }

    [[nodiscard]] inline const T &operator[](size_t i) const {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return storage_.data(i);
    }

    [[nodiscard]] T &at(size_t i) {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return (*this)[i];
    }

    [[nodiscard]] const T &at(size_t i) const {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return (*this)[i];
    }

    [[nodiscard]] inline T &front() {
        return (*this)[0];
    }

    [[nodiscard]] inline const T &front() const {
        return (*this)[0];
    }

    [[nodiscard]] inline T &back() {
        return (*this)[size() - 1];
    }

    [[nodiscard]] inline const T &back() const {
        return (*this)[size() - 1];
    }

    [[nodiscard]] T &at_front() {
        if (empty()) throw std::range_error("can't take front() from ArrayT of size 0");

        return front();
    }

    [[nodiscard]] const T &at_front() const {
        if (empty()) throw std::range_error("can't take front() from ArrayT of size 0");

        return front();
    }

    [[nodiscard]] T &at_back() {
        if (empty()) throw std::range_error("can't take back() from ArrayT of size 0");

        return back();
    }

    [[nodiscard]] const T &at_back() const {
        if (empty()) throw std::range_error("can't take back() from ArrayT of size 0");

        return back();
    }

    [[nodiscard]] inline T *data() {
        static_assert(IndexedStorageT<T>::can_give_data_ptr);
        return storage_.data_ptr();
    }

    [[nodiscard]] inline const T *data() const {
        static_assert(IndexedStorageT<T>::can_give_data_ptr);
        return storage_.data_ptr();
    }

// ============================================================================ capacity

    [[nodiscard]] constexpr inline size_t size() const {
        return storage_.size();
    }

    [[nodiscard]] constexpr inline bool empty() const {
        return !!storage_.size();
    }

    void reserve(size_t capacity) {
        storage_.reserve(capacity);
    }

    [[nodiscard]] size_t capacity() const {
        return storage_.capacity();
    }

    void shrink_to_fit() {
        storage_.shrink_to_fit();
    }

// ============================================================================ iterators

    auto begin() {
        if constexpr (IndexedStorageType::provides_iterators) {
            return storage_.begin();
        } else {
            return Iterator<ArrayT, T>(*this, 0);
        }
    }

    auto end() {
        if constexpr (IndexedStorageType::provides_iterators) {
            return storage_.end();
        } else {
            return Iterator<ArrayT, T>(*this, size());
        }
    }

    auto begin() const {
        if constexpr (IndexedStorageType::provides_iterators) {
            return storage_.begin();
        } else {
            return Iterator<ArrayT, T>(*this, 0);
        }
    }

    auto end() const {
        if constexpr (IndexedStorageType::provides_iterators) {
            return storage_.end();
        } else {
            return Iterator<ArrayT, T>(*this, size());
        }
    }

    // using iterator = decltype(std::declval<ArrayType>().begin());
    // using const_iterator = decltype(std::declval<const ArrayType>().begin());

    // using iterator = std::conditional_t<IndexedStorageType::provides_iterators, decltype(storage_.begin()), Iterator<ArrayType, T>>;
    // using const iterator = std::conditional_t<IndexedStorageType::provides_iterators, const decltype(storage_.begin()), Iterator<const ArrayType, const T>>;

// ============================================================================ modifiers

    template <typename ElemT>
    void push_back(ElemT &&elem) {
        static_assert(IndexedStorageT<T>::can_modify_size);

        T *placement = storage_.expand_one();
        new(placement) T(std::forward<ElemT>(elem));
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

    void swap(ArrayT<T, IndexedStorageT> &other) {
        std::swap(storage_, other.storage_);
    }

// ============================================================================ addons

    void fill(const T &elem) {
        for (size_t i = 0; i < size(); ++i) {
            storage_.data(i) = elem;
        }
    }

    template <typename P>
    void filter(const P &predicate) {
        size_t write_idx = 0;
        size_t read_idx = 0;
        
        while (read_idx < size()) {
            if (predicate((*this)[read_idx])) {
                (*this)[write_idx] = std::move((*this)[read_idx]);
                ++write_idx;
            }

            ++read_idx;
        }

        resize(write_idx);
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

    static size_t get_byte_size(size_t elem_size) {
        return elem_size / 8 + (elem_size % 8 != 0);
    }

public:
    BoolProxy operator[](size_t i) {
        return BoolProxy(BaseArray::storage_.data(i / 8), i % 8);
    }

    ArrayT() :
        ArrayT<uint8_t, IndexedStorageT>(),
        bools_cnt(IndexedStorageT<uint8_t>::is_dynamic ? 0 : BaseArray::storage_.size())
    {}

    ArrayT(size_t length, const bool elem={}) :
        ArrayT<uint8_t, IndexedStorageT>(get_byte_size(length), elem),
        bools_cnt(length)
    {}

// ============================================================================ element access

    BoolProxy at(size_t i) {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return (*this)[i];
    }

    const BoolProxy at(size_t i) const {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of ArrayT (size = " + std::to_string(size()) + ")");
        }

        return (*this)[i];
    }

    inline BoolProxy front() {
        return (*this)[0];
    }

    inline const BoolProxy front() const {
        return (*this)[0];
    }

    inline BoolProxy back() {
        return (*this)[size() - 1];
    }

    inline const BoolProxy back() const {
        return (*this)[size() - 1];
    }

    BoolProxy at_front() {
        if (empty()) throw std::range_error("can't take front() from ArrayT of size 0");

        return front();
    }

    const BoolProxy at_front() const {
        if (empty()) throw std::range_error("can't take front() from ArrayT of size 0");

        return back();
    }

    const BoolProxy at_back() const {
        if (empty()) throw std::range_error("can't take back() from ArrayT of size 0");

        return back();
    }

// ============================================================================ capacity

    constexpr inline size_t size() const {
        return bools_cnt;
    }

    constexpr inline bool empty() const {
        return !!bools_cnt;
    }

    void reserve(size_t capacity) {
        BaseArray::storage_.reserve(get_byte_size(capacity));
    }

    size_t capacity() const {
        return BaseArray::storage_.capacity() * 8;
    }

// ============================================================================ modifiers

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

    void clear() {
        BaseArray::clear();
        bools_cnt = 0;
    }

    void resize(size_t new_size, const bool fill_elem={}) {
        static_assert(IndexedStorageT<bool>::can_modify_size);

        if (BaseArray::storage_.size()) {
            uint8_t fillmask_last = ((uint8_t) - (int) fill_elem) << (bools_cnt % 8);
            BaseArray::storage_.data(BaseArray::storage_.size() - 1) |= fillmask_last;
        }

        BaseArray::storage_.resize(get_byte_size(new_size), fill_elem ? (uint8_t) -1 : 0);
        bools_cnt = new_size;
    }

    void fill(const bool &elem) {
        uint8_t fill_elem = elem ? (uint8_t) -1 : 0;
        for (size_t i = 0; i < BaseArray::storage_.size(); ++i) {
            BaseArray::storage_.data(i) = fill_elem;
        }
    }
};


template <typename T, template <typename U> typename Allocator=kctf::allocator::Simple>
using Vector = ArrayT<T, storage::IndexedDynamic<Allocator>::template type>;

template <typename T, int Size>
using Array = ArrayT<T, storage::IndexedStatic<Size>::template type>;

template <typename T, int ChunkSize, template <typename U> typename Allocator=kctf::allocator::Simple>
using ChunkVector = ArrayT<T, storage::IndexedChunked<ChunkSize, Allocator>::template type>;

} // namespace kctf
