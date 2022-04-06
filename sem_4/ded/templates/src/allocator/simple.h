#pragma once


namespace kctf::allocator {


template<typename T>
class Simple {
public:
    using value_type = T;

    Simple()  {}
    Simple(const Simple<T> &) noexcept {}
    Simple(Simple<T> &&) noexcept {}

    Simple &operator=(const Simple<T> &) noexcept { return *this; }
    Simple &operator=(Simple<T> &&) noexcept { return *this; }

    template<typename U>
    Simple(const Simple<U> &) noexcept {}

    template<typename U>
    Simple &operator=(const Simple<U> &) noexcept { return *this; }

    ~Simple() {}

    T* allocate(size_t n) {
        return (T*) new char[n * sizeof(T)];
    }

    void deallocate(T* ptr, size_t /*n*/) noexcept {
        delete[] ptr;
    }

    bool operator==(const Simple<T> &) noexcept {
        return true;
    }

    bool operator!=(const Simple<T> &other) noexcept {
        return !(*this == other);
    }
    
};


}
