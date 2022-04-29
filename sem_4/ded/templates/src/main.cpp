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

kctf::String &lengthen(kctf::String &str, size_t cnt) {
    char c = 'a';
    for (size_t i = 0; i < cnt; ++i) {
        str += c;
        ++c;
        if (c > 'z') {
            c = 'a';
        }
    }

    return str;
}

int main() {
    logger.set_verb_level(Logger::Level::info);

    String to_look_at = "Some long string\n";

    constexpr size_t LOOKS_CNT = 10000;
    String others[LOOKS_CNT];
    for (size_t i = 0; i < LOOKS_CNT; ++i) {
        others[i] = to_look_at;
    }

// ============================================================================ String

    // String str = "smol";    print(str);
    // String s1 = str;

    // lengthen(str, 2);       print(str);
    // String s2 = str;

    // lengthen(str, 4);       print(str);
    // String s3 = str;

    // str += "!!!";
    // lengthen(str, 40);
    // print(str, ']');

    // print(s1);
    // print(s2);
    // print(s3);


    return 0;
}
