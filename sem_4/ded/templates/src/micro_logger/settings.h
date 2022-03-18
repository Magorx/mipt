#pragma once


#include "utils/rgba/rgba.h"


#define CLR_BACKGROUND RGBA("FFFFFF")
#define CLR_FONT RGBA("000000")
#define CLR_KCTF RGBA("29C329")

#define CLR_IMPORTANT_BAD RGBA("EE1C1C")
#define CLR_IMPORTANT_GOOD RGBA("108910")

#define CLR_FLOW RGBA("278EC6")
#define CLR_CTOR RGBA("73914F")
#define CLR_DTOR RGBA("BE8607")
#define CLR_OP RGBA("D324D3")

#define CLR_FUNC RGBA("4242C3")

#define CLR_VAR_NORMAL RGBA("D4EACF")
#define CLR_VAR_BAD RGBA("EACFCF")


inline bool is_important_bad(Operator op) {
    return op == Operator::asgn_copy
        || op == Operator::ctor_copy;
}

inline bool is_important_good(Operator op) {
    return op == Operator::asgn_move
        || op == Operator::ctor_move;
}

inline bool is_ctor(Operator op) {
    return op == Operator::ctor
        || op == Operator::ctor_copy
        || op == Operator::ctor_move;
}

inline bool is_dtor(Operator op) {
    return op == Operator::dtor;
}


inline std::string get_op_color(Operator op) {
    if (is_important_bad(op)) {
        return to_string(CLR_IMPORTANT_BAD);
    } else if (is_important_good(op)) {
        return to_string(CLR_IMPORTANT_GOOD);
    } else if (is_ctor(op)) {
        return to_string(CLR_CTOR);
    } else if (is_dtor(op)) {
        return to_string(CLR_DTOR);
    } else {
        return to_string(CLR_OP / 2);
    }
}
