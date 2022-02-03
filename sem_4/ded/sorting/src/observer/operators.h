#pragma once


enum Operator {
    add,
    sub,
    mul,
    div,
    mod,

    un_add,
    un_sub,

    pref_add,
    pref_sub,
    post_add,
    post_sub,

    eq,
    ne,
    lt,
    le,
    gt,
    ge,

    b_not,
    b_and,
    b_or,
    b_xor,
    b_ls,
    b_rs,

    l_neg,
    l_and,
    l_or,

    asgn,
    asgn_add,
    asgn_sub,
    asgn_mul,
    asgn_div,
    asgn_mod,
    asgn_b_and,
    asgn_b_or,
    asgn_b_xor,
    asgn_b_sl,
    asgn_b_sr,

    subscript,
    indirection,
    addr_of,
    struct_deref,

    call,
    comma,

    a_new,
    a_new_arr,
    a_del,
    a_del_arr,

    ctor,
    ctor_copy,
    copy,

    MAX
};

const char *to_str(Operator op) {
    switch (op) {
        case add:
            return "+";
        case sub:
            return "-";
        case mul:
            return "*";
        case Operator::div:
            return "/";
        case mod:
            return "%";

        case un_add:
            return "+.";
        case un_sub:
            return "-.";

        case pref_add:
            return "++.";
        case pref_sub:
            return "--.";
        case post_add:
            return ".++";
        case post_sub:
            return ".++";

        case eq:
            return "==";
        case ne:
            return "!=";
        case lt:
            return "<";
        case le:
            return "<=";
        case gt:
            return ">";
        case ge:
            return ">=";

        case b_not:
            return "~";
        case b_and:
            return "&";
        case b_or:
            return "|";
        case b_xor:
            return "^";
        case b_ls:
            return "<<";
        case b_rs:
            return ">>";

        case l_neg:
            return "!";
        case l_and:
            return "&&";
        case l_or:
            return "||";

        case asgn:
            return "=";
        case asgn_add:
            return "+=";
        case asgn_sub:
            return "-=";
        case asgn_mul:
            return "*=";
        case asgn_div:
            return "/=";
        case asgn_mod:
            return "%=";
        case asgn_b_and:
            return "&=";
        case asgn_b_or:
            return "|=";
        case asgn_b_xor:
            return "^=";
        case asgn_b_sl:
            return "<=";
        case asgn_b_sr:
            return ">=";

        case subscript:
            return "[]";
        case indirection:
            return "*";
        case addr_of:
            return "&";
        case struct_deref:
            return "->";

        case call:
            return "()";
        case comma:
            return ",";

        case a_new:
            return "new";
        case a_new_arr:
            return "new[]";
        case a_del:
            return "delete";
        case a_del_arr:
            return "delete[]";
        
        case ctor:
            return "ctor";
        case ctor_copy:
            return "ctor_copy";
        case copy:
            return "copy";
        
        default:
            return "ERR_OP";
    }
}
