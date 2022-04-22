#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <algorithm>

#include "utils/Utils.h"
#include "array/array.h"
#include "vec/vec.h"

#include "print.h"

#include "string/string.h"

using kctf::String;


int main() {
    logger.set_verb_level(Logger::Level::info);

// ============================================================================ String

    String str("aboba", 5);
    String s2 = str + '!';

    for (auto c : s2) {
        print<' ', ' '>(c);
    }
    print();

    for (size_t i = 0; i <  s2.size(); ++i) {
        print<' ', ' '>(s2[i]);
    }
    print();
    

    return 0;
}
