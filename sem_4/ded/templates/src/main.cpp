#include <cstdlib>
#include <cstdio>

#include <iostream>

#include "utils/Utils.h"
#include "array/array.h"
#include "vec/vec.h"

#include "print.h"


template <typename T>
struct Box {
    T a;
    T b;

    Box(const T &a, const T &b) :
    a(a),
    b(b)
    {}

    ~Box() {
        printf("box[%d, %d] destroyed\n", a, b);
    }
};


std::string observed_int_log(const Observed<Box<int>> &obj) {
    return "[" + std::to_string(obj.get_data().a) + ", " + std::to_string(obj.get_data().b) + "]";
}

int main() {
    logger.set_verb_level(Logger::Level::info);

    Observed<Box<int>>::set_one_line_log(observed_int_log);

    MicroLogger<Box<int>> microlog;

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

    kctf::Array<Observed<Box<int>>, kctf::storage::IndexedChunk> arr(0, Observed<Box<int>>{Box{-1, -1}});

    // for (size_t i = 6; i < arr.size(); ++i) {
    //     arr[i] = Box{(int) i, (int) i % 3 + 1};
    // }

    for (size_t i = 0; i < 5; ++i) {
        arr.push_back(Observed<Box<int>>{Box{(int) i, (int) i % 3 + 1}});
    }
    printf("stop\n");

    for (size_t i = 0; i < arr.size(); ++i) {
        // print<' ', ' '>((int) arr[i].a);
    }
    print();

// ============================================================================ Vector

    // Vec<int, 4> a(1, 2, 3, 4);
    // Vec<int, 4> b(4, 3, 2, 1);

    // print(a.dot(b));

    return 0;
}
