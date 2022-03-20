#include <cstdlib>
#include <cstdio>

#include <iostream>

#include "utils/Utils.h"
#include "array/array.h"
#include "vec/vec.h"

#include "print.h"


template <typename T>
struct Box {
    T value_;

    Box(const T &value) : value_(value) {}

    ~Box() {
        print((*this), "destroyed");
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

    // Observed<Box<int>>::set_one_line_log([](const Observed<Box<int>> &obj) {
    //     return "[" + std::to_string(obj.get_data().a) + ", " + std::to_string(obj.get_data().b) + "]";
    // });
    // MicroLogger<Box<int>> microlog;

// ============================================================================ Array Dynamic

//     for (size_t i = 0; i < arr.size(); ++i) {
//         print<' ', ' '>(arr[i]);
//     }
//     print();

// ============================================================================ Array Static

//     kctf::Array<bool, kctf::storage::IndexedStatic<10>::type> arr;
    
//     for (size_t i = 0; i < arr.size(); ++i) {
//         arr[i] = i % 3;
//     }

//     // for (size_t i = 0; i < 10; ++i) { |
//     //     arr.push_back(true);          | <-- Compilation error - can't extend StaticStorage
//     // }                                 |

//     for (size_t i = 0; i < arr.size(); ++i) {
//         print<' ', ' '>(arr[i]);
//     }
//     print();

// ============================================================================ Array Chunk

    kctf::Vector<int> arr_vec;
    arr_vec.resize(3, 1);
    arr_vec.push_back(11);
    print("arr_vec | size is", arr_vec.size(), " capacity is", arr_vec.capacity());

    kctf::ChunkVector<int, 2> arr_chunk(2, -2);
    arr_chunk.push_back(2);
    arr_chunk.reserve(10);
    arr_chunk.resize(7, 2);
    // print("arr_chunk | size is", arr_chunk.size(), " capacity is", arr_chunk.capacity());

    kctf::Array<int, 10> arr_stat;
    arr_stat.fill(3);

    print("arr_vec size:", arr_vec.size());
    for (size_t i = 0; i < arr_vec.size(); ++i) {
        print<' ', ' '>(arr_vec[i]);
    }
    print();

    kctf::Vector<int> arr_cpy;
    print(arr_cpy.data(), "!!!!!!!!!");
    std::swap(arr_cpy, arr_vec);

    print("arr_cpy size:", arr_cpy.size());
    for (size_t i = 0; i < arr_cpy.size(); ++i) {
        print<' ', ' '>(arr_cpy[i]);
    }
    print();

    print(arr_cpy.data());

// ============================================================================ Vector

    // Vec<int, 4> a(1, 2, 3, 4);
    // Vec<int, 4> b(4, 3, 2, 1);

    // print(a.dot(b));

    return 0;
}
