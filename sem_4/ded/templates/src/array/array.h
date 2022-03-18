#pragma once


#include "storage/Indexed.h"


namespace kctf {


template <
          typename T,
          template <typename U> typename IndexedStorageT
         >
class Array {
protected:
    IndexedStorageT<T> storage_;

    // template <typename PushBackT>
    // constexpr void validate_data_type(PushBackT &&) {
    //     static_assert(std::is_same_v<std::remove_cvref_t<PushBackT>, T>);
    // }
    
public:
    Array() : storage_() {
    }

    Array(size_t length, const T &elem={}) : storage_(length, elem) {
    }

    T &operator[](size_t i) {
        return storage_.data(i);
    }

    template <typename Q>
    void push_back(Q &&elem) {
        T *placement = storage_.expand_one();
        new(placement) T(std::forward<Q>(elem));
    }

    template <typename ... Ts>
    void emplace_back(Ts ... vs) {
        T *placement = storage_.expand_one();
        new(placement) T(std::forward<Ts>(vs)...);
    }

    constexpr inline size_t size() const {
        return storage_.size();
    }

    constexpr inline bool empty() const {
        return !!storage_.size();
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
class Array<bool, IndexedStorageT> : private Array<uint8_t, IndexedStorageT> {
    size_t bools_cnt; 

    using BaseArray = Array<uint8_t, IndexedStorageT>;
public:
    BoolProxy operator[](size_t i) {
        return BoolProxy(BaseArray::storage_.data(i / 8), i % 8);
    }

    Array() :
        Array<uint8_t, IndexedStorageT>(),
        bools_cnt(IndexedStorageT<uint8_t>::is_dynamic ? 0 : BaseArray::storage_.size())
    {}

    Array(size_t length, const bool elem={}) :
        Array<uint8_t, IndexedStorageT>(length / 8 + 1 * (!!(length % 8 != 0)), elem),
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

} // namespace kctf
