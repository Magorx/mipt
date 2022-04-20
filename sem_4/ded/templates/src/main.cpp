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

    String str = "aboba";
    str += "aaa" + str + "ccc" + "!!!";

    for (auto c : str) {
        print<' ', ' '>(c);
    }
    print();

    

    return 0;
}
