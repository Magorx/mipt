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

    kctf::Vector<int> a(3);
    a.reserve(100);
    print("size is", a.size(), " capacity is", a.capacity());

    kctf::ChunkVector<int> b(0);
    b.reserve(101);
    b.shrink_to_fit();
    print("size is", b.size(), " capacity is", b.capacity());

    // for (size_t i = 0; i < arr.size(); ++i) {
    //     arr[i] = -((int)i % 3);
    // }

    // for (size_t i = 0; i < 160; ++i) {
    //     arr.push_back(i % 3);
    // }
    // for (size_t i = 0; i < 7; ++i) {
    //     arr.pop_back();
    // }

    // print("arr size:", arr.size());
    // for (size_t i = 0; i < arr.size(); ++i) {
    //     print<' ', ' '>(arr[i]);
    // }
    // print();

// ============================================================================ Vector

    // Vec<int, 4> a(1, 2, 3, 4);
    // Vec<int, 4> b(4, 3, 2, 1);

    // print(a.dot(b));

    return 0;
}
