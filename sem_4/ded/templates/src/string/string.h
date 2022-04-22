#pragma once


#include "string_core.h"


namespace kctf {

template <typename CharT, template <typename U>  typename Allocator = allocator::Simple>
class StringT : public StringCore<CharT, Allocator> {
    using Core = StringCore<CharT, Allocator>;

public:
    using iterator = CharT*;
    using const_iterator = const CharT*;

    StringT() {}
    
    explicit StringT(size_t length, const CharT &elem={})
        : Core(length, elem) {}

    explicit StringT(std::initializer_list<CharT> list)
        : Core(list) {}

    StringT(const StringT &other)
        : Core(other) {}
    
    StringT &operator=(const StringT &other) {
        Core::operator=(other);

        return *this;
    }

    StringT(StringT &&other)
        : Core(other) {}
    
    StringT &operator=(const StringT &&other) {
        Core::operator=(other);

        return *this;
    }

// ============================================================================ from CharT *
    StringT(const CharT *str, size_t capacity)
        : Core() {
        for (size_t i = 0; i < capacity; ++i) {
            append(str[i]);
        }
    }

    StringT(const CharT *str)
        : StringT(str, strlen(str)) {}
    
    StringT(const std::string str)
        : StringT(str.c_str(), str.size())) {}

// ============================================================================ to

    explicit operator std::string() {
        return {data(), size()};
    }

// ============================================================================ expansion
    inline void append(const CharT c) {
        *Core::expand_one() = c;
    }

    void append(const CharT *str, size_t len) {
        reserve(size() + len);

        for (size_t i = 0; i < len; ++i) {
            *Core::expand_one() = str[i];
        }
    }

    StringT &operator+=(CharT c) {
        append(c);

        return *this;
    }

    StringT &operator+=(const StringT &str) {
        auto data = str.data();
        append(data, strlen(data));

        return *this;
    }

    StringT &operator+=(const CharT *str) {
        append(str, strlen(str));

        return *this;
    }

    StringT &&operator+(const CharT *str) && {
        *this += str;
        return std::move(*this);
    }

    StringT operator+(const CharT *str) const & {
        StringT copy = *this;
        copy += str;
        return copy;
    }

    StringT &&operator+(CharT c) && {
        *this += c;
        return std::move(*this);
    }

    StringT operator+(CharT c) const & {
        StringT copy = *this;
        copy += c;
        return copy;
    }

    friend StringT operator+(const CharT *str, const StringT &obj) {
        StringT copy = str;
        copy += obj;
        return copy;
    }

    friend StringT operator+(CharT c, const StringT &obj) {
        StringT copy;
        copy += c;
        copy += obj;
        return copy;
    }

// ============================================================================ element access

    [[nodiscard]] inline CharT &operator()(size_t i) {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of StringT (size = " + std::to_string(size()) + ")");
        }

        return Core::data_at(i);
    }

    [[nodiscard]] inline const CharT &operator[](size_t i) const {
        printf("const\n");
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of StringT (size = " + std::to_string(size()) + ")");
        }

        return Core::data_at(i);
    }

    [[nodiscard]] CharT &at(size_t i) {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of StringT (size = " + std::to_string(size()) + ")");
        }

        return (*this)[i];
    }

    [[nodiscard]] const CharT &at(size_t i) const {
        if (i >= size()) {
            throw std::range_error("Bad index (" + std::to_string(i) + ") passed to operator[] of StringT (size = " + std::to_string(size()) + ")");
        }

        return (*this)[i];
    }

    [[nodiscard]] inline CharT &front() {
        return (*this)[0];
    }

    [[nodiscard]] inline const CharT &front() const {
        return (*this)[0];
    }

    [[nodiscard]] inline CharT &back() {
        return (*this)[size() - 1];
    }

    [[nodiscard]] inline const CharT &back() const {
        return (*this)[size() - 1];
    }

    [[nodiscard]] inline CharT *data() {
        return Core::data_ptr();
    }

    [[nodiscard]] inline const CharT *data() const {
        return Core::data_ptr();
    }

// ============================================================================ capacity

    [[nodiscard]] constexpr inline size_t size() const {
        return Core::size();
    }

    [[nodiscard]] constexpr inline bool empty() const {
        return !!Core::size();
    }

    void reserve(size_t capacity) {
        Core::reserve(capacity);
    }

    [[nodiscard]] size_t capacity() const {
        return Core::capacity();
    }

    void shrink_to_fit() {
        Core::shrink_to_fit();
    }

// ============================================================================ iterators

    auto begin() {
        return Core::data_ptr();
    }
    
    auto begin() const {
        return Core::data_ptr();
    }

    auto end() {
        return Core::data_ptr() + size();
    }

    auto end() const {
        return Core::data_ptr() + size();
    }

    auto rbegin() {
        return std::make_reverse_iterator(begin());
    }
    
    auto rbegin() const {
        return std::make_reverse_iterator(begin());
    }

    auto rend() {
        return std::make_reverse_iterator(end());
    }

    auto rend() const {
        return std::make_reverse_iterator(end());
    }

// ============================================================================ modifiers
    void pop_back() {
        Core::extract_one();
    }

    void clear() {
        Core::clear();
    }

    void resize(size_t new_size, const CharT &fill_elem={}) {
        Core::resize(new_size, fill_elem);
    }

    void swap(StringT<CharT, Allocator> &other) {
        std::swap(reinterpret_cast<Core&>(*this), reinterpret_cast<Core&>(other));
    }

// ============================================================================ addons

    void fill(CharT elem) {
        for (size_t i = 0; i < size(); ++i) {
            (*this)[i] = elem;
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



using String = StringT<char>;

}
