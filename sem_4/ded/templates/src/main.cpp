#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <algorithm>

#include "utils/Utils.h"
#include "vec/vec.h"

#include "print.h"

#include "function/function.h"


int func(char c) {
    printf("Fnkc called with [%c]\n", c);
    return c * 10;
}

struct Callable {
    int x;

    int operator()(char c) {
        printf("Clss called with [%c]\n", c);
        return x;
    }

    int method(char c) {
        return c + 7;
    }
};


using kctf::Function;


int main() {
    logger.set_verb_level(Logger::Level::info);

    Callable obj {42};

    std::vector<Function<int (char)>> arr(7);
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i % 2) {
            arr[i] = func;
        } else {
            arr[i] = obj;
        }
    }

    for (auto &f : arr) {
        f('A');
    }

    kctf::Function<int (Callable*, char)> method(&Callable::method);
    method(&obj, 'A');

    // Function f1 = func;
    // Function f2 = obj;

    // Function f3 = [](char c) { printf("Lmbd called with [%c]\n", c); return -1; };
    // Function f4 = [&obj](char c) { printf("Lmbd called with [%c] and obj += 1\n", c); obj.x += 1; return -1; };

    // print(f1('a'));
    // print(f2('b'));
    // print(f3('c'));

    // print(f4('d'));
    // print(f4('d'));
    // print(f4('d'));
    // print("Cur obj:", obj.x);
    
    // print(f2('b'));

    return 0;
}
