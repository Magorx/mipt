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
    String s(str);
    String s1(str);
    String s2(str);
    String s3(str);

    printf("===\n");
    s2(1) = '@';
    s2(2) = '!';
 
    // for (auto c : s2) {
    //     print<' ', ' '>(c);
    // }
    // print();
    printf("[%c]\n", s2[1]);

    // int n;
    // scanf("%d", &n);

    for (size_t i = 0; i <  s2.size(); ++i) {
        print<' ', ' '>(s2[i]);
    }
    print();

    // for (size_t i = 0; i <  str.size(); ++i) {
    //     print<' ', ' '>(str[i]);
    // }
    // print();
    

    return 0;
}
