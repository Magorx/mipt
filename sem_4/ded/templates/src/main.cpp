#include <cstdlib>
#include <cstdio>

#include <iostream>

#include "utils/Utils.h"
#include "array/array.h"
#include "vec/vec.h"

#include "print.h"


template <typename T>
struct BoxND {
    T value_;

    BoxND(const T &value) : value_(value) {}

    ~BoxND() {
    }

    operator T() {
        return value_;
    }

    friend std::ostream& operator<< (std::ostream& stream, const BoxND &box) {
        stream << "BoxND[" << box.value_ << ']';
        return stream;
    }
};

template <typename T>
struct Box {
    T value_;

    Box() : value_() {}

    Box(const T &value) : value_(value) {}

    ~Box() {
    }

    operator T() {
        return value_;
    }

    friend std::ostream& operator<< (std::ostream& stream, const Box &box) {
        stream << "Box[" << box.value_ << ']';
        return stream;
    }
};

int main() {
    logger.set_verb_level(Logger::Level::info);

// ============================================================================ Arrays

    kctf::Vector<int> arr_vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    kctf::ChunkVector<int, 2> arr_chunk(100, 7);
    arr_chunk.push_back(2);
    arr_chunk.reserve(10);
    arr_chunk.resize(7, 2);

    kctf::Array<Box<int>, 10> arr_stat = {1, 2, 3, 4, 5};

    kctf::Vector<bool> arr_bool(5);
    arr_bool.resize(20, 0);

    // ============ something
    
    arr_vec.filter([](int x){ return x % 2; });

    // ============ output

    printf("arr_bool |  size: %zu | capacity: %zu\n", arr_bool.size(), arr_bool.capacity());
    for (size_t i = 0; i < arr_bool.size(); ++i) {
        print<' ', ' '>(arr_bool[i]);
    }
    print();

    printf("arr_vec |  size: %zu | capacity: %zu\n", arr_vec.size(), arr_vec.capacity());
    for (size_t i = 0; i < arr_vec.size(); ++i) {
        print<' ', ' '>(arr_vec[i]);
    }
    print();

    printf("arr_chunk |  size: %zu | capacity: %zu\n", arr_chunk.size(), arr_chunk.capacity());
    for (size_t i = 0; i < arr_chunk.size(); ++i) {
        print<' ', ' '>(arr_chunk[i]);
    }
    print();

    printf("arr_stat |  size: %zu | capacity: %zu\n", arr_stat.size(), arr_stat.capacity());
    for (size_t i = 0; i < arr_stat.size(); ++i) {
        print<' ', ' '>(arr_stat[i]);
    }
    print();

    kctf::Vector<int> arr_cpy;
    print("arr_cpy size:", arr_cpy.size());
    for (size_t i = 0; i < arr_cpy.size(); ++i) {
        print<' ', ' '>(arr_cpy[i]);
    }
    print();

// ============================================================================ Vector

    // Vec<int, 4> a(1, 2, 3, 4);
    // Vec<int, 4> b(4, 3, 2, 1);

    // print(a.dot(b));

    return 0;
}
