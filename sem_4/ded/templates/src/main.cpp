#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <algorithm>

#include "utils/Utils.h"
#include "array/array.h"
#include "vec/vec.h"

#include "print.h"

#include "function/function.h"


int func(char c) {
    printf("Func  called with [%c]\n", c);
    return c * 10;
}

struct Callable {
    int x;

    int operator()(char c) {
        printf("Class called with [%c]\n", c);
        return x;
    }
};


using kctf::Function;


int main() {
    logger.set_verb_level(Logger::Level::info);

    Callable obj {42};

    Function f1 = func;
    Function f2 = obj;

    Function f3 = [](char c) { printf("Lambd called with [%c]\n", c); return -1; };
    Function f4 = [&obj](char c) { printf("Lambd called with [%c] and obj += 1\n", c); obj.x += 1; return -1; };

    print(f1('a'));
    print(f2('b'));
    print(f3('c'));

    print(f4('d'));
    print(f4('d'));
    print(f4('d'));
    print("Cur obj:", obj.x);
    
    print(f2('b'));

    return 0;
}
