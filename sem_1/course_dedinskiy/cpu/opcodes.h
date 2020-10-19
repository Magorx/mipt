#ifndef ASSEMBER_COMMANDS_H
#define ASSEMBER_COMMANDS_H

#include <stdint.h>

const int16_t VERSION = 1;

enum CPU_SETTINGS {
	REGISTERS_COUNT = 4
};

enum VALUE_TYPES {
	VALUE_CONSTANT = 16,
	VALUE_REGISTER = 32,
	VALUE_LABEL    = 48
};

enum OPCODES_CHAR_LITERALS {
	OPCODE_SHORT = 's',
	OPCODE_INT = 'i',
	OPCODE_LONG = 'l',
	OPCODE_UNSIGNED = 'u',
	OPNAME_DOUBLE = 'd',
	OPNAME_PLUS = '+',
	OPNAME_MINUS = '-',
	OPNAME_MULTIPLY = '*',
	OPNAME_DIVIDE = '/'
};
const unsigned char *OPERATIONS = (unsigned char*)"+*";

const char *OPNAME_PUSH = "push";
const char *OPNAME_POP  = "pop";
const char *OPNAME_ADD  = "add";
const char *OPNAME_SUB  = "sub";
const char *OPNAME_MUL  = "mul";
const char *OPNAME_DIV  = "div";
const char *OPNAME_SIN  = "sin";
const char *OPNAME_COS  = "cos";
const char *OPNAME_OUT  = "out";

const int MAX_COMMAND_ARGS_COUNT = 3;
const size_t MAX_OPCODES_COUNT = 256;

enum OPCODES {
   OPCODE_PUSH   = 1,
   OPCODE_POP    = 2,
   OPCODE_ADD    = 10,
   OPCODE_SUB    = 11,
   OPCODE_MUL    = 12,
   OPCODE_DIV    = 13,
   OPCODE_SIN    = 20,
   OPCODE_COS    = 21,
   OPCODE_SQRT   = 22,
   OPCODE_IN     = 50,
   OPCODE_OUT    = 51,
   OPCODE_JMP    = 101,
   OPCODE_JA     = 102,
   OPCODE_JAE    = 103,
   OPCODE_JB     = 104,
   OPCODE_JBE    = 105,
   OPCODE_JE     = 106,
   OPCODE_JNE    = 107,
   OPCODE_CALL   = 108,
   OPCODE_RET    = 109,
   OPCODE_HALT   = 255,
};

const char *OPNAMES[] = {
    [1  ] = "push",
    [2  ] = "pop",
    [10 ] = "add",
    [11 ] = "sub",
    [12 ] = "mul",
    [13 ] = "div",
    [20 ] = "sin",
    [21 ] = "cos",
    [22 ] = "sqrt",
    [50 ] = "in",
    [51 ] = "out",
    [101] = "jmp",
    [102] = "ja",
    [103] = "jae",
    [104] = "jb",
    [105] = "jbe",
    [106] = "je",
    [107] = "jne",
    [108] = "call",
    [109] = "ret",
    [255] = "halt",
};

const byte OPARGS[] = {
    [1  ] = 1,
    [2  ] = 1,
    [10 ] = 0,
    [11 ] = 0,
    [12 ] = 0,
    [13 ] = 0,
    [20 ] = 0,
    [21 ] = 0,
    [22 ] = 0,
    [50 ] = 0,
    [51 ] = 0,
    [101] = VALUE_LABEL,
    [102] = VALUE_LABEL,
    [103] = VALUE_LABEL,
    [104] = VALUE_LABEL,
    [105] = VALUE_LABEL,
    [106] = VALUE_LABEL,
    [107] = VALUE_LABEL,
    [108] = VALUE_LABEL,
    [109] = 0,
    [255] = 0,
};

#endif 
