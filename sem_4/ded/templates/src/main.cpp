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


// void print(const kctf::String &str) {
//     for (size_t i = 0; i < str.size(); ++i) {
//         putchar(str[i]);
//     }
//     putchar('\n');
// }

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
