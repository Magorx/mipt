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

    String str = "Some nice long string.";

    str.become_view();

    // for (auto c : s2) {
    //     print<' ', ' '>(c);
    // }
    // print();

    for (size_t i = 0; i <  str.size(); ++i) {
        print<' ', ' '>(str[i]);
    }
    print();
    

    return 0;
}
