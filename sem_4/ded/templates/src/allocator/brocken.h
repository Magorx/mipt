#pragma once


namespace kctf::allocator {


template<typename T>
class Brocken {
public:
    using value_type = T;

    Brocken()  {}
    Brocken(const Brocken<T> &) noexcept {}
    Brocken(Brocken<T> &&) noexcept {}

    Brocken &operator=(const Brocken<T> &) noexcept { return *this; }
    Brocken &operator=(Brocken<T> &&) noexcept { return *this; }

    template<typename U>
    Brocken(const Brocken<U> &) noexcept {}

    template<typename U>
    Brocken &operator=(const Brocken<U> &) noexcept { return *this; }

    ~Brocken() {}

    T* allocate(size_t /*n*/) {
        return nullptr;
    }

    void deallocate(T* /*ptr*/, size_t /*n*/) noexcept {}

    bool operator==(const Brocken<T> &) noexcept {
        return true;
    }

    bool operator!=(const Brocken<T> &other) noexcept {
        return !(*this == other);
    }
    
};


}
