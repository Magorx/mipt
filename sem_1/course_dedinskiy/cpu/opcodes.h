#ifndef ASSEMBER_COMMANDS_H
#define ASSEMBER_COMMANDS_H

#include <stdint.h>

const int16_t VERSION = 1;
const Byte REGISTERS_COUNT = 4;

/*
Command - index

push - 1
pop - 2
add - 3
sub - 4
mul - 5
div - 6
sin - 7
cos - 8
out - 9

*/

enum OPCODES {
	OPCODE_PUSH = 1,
	OPCODE_POP,
	OPCODE_ADD,
	OPCODE_SUB,
	OPCODE_MUL,
	OPCODE_DIV,
	OPCODE_SIN,
	OPCODE_COS,
	OPCODE_OUT
};

enum OPCODES_ARGS {
	VALUE_REGISTER = 1,
	VALUE_CONSTANT
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
const unsigned char *OPERATIONS = (unsigned char*)"+-*/";

const char *OPNAME_PUSH = "push";
const char *OPNAME_POP  = "pop";
const char *OPNAME_ADD  = "add";
const char *OPNAME_SUB  = "sub";
const char *OPNAME_MUL  = "mul";
const char *OPNAME_DIV  = "div";
const char *OPNAME_SIN  = "sin";
const char *OPNAME_COS  = "cos";
const char *OPNAME_OUT  = "out";

const char *COMMANDS_INDEXES[] = {
	""
	"push",
	"pop",
	"add",
	"sub",
	"mul",
	"div",
	"sin",
	"cos",
	"out",
	"halt"
};

const char COMMANDS_ARGS_COUNT[] = {
	-1,
	3,
	1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

const int MAX_COMMANDS_ARGS_COUNT = 3;


#endif