#ifndef ASSEMBER_COMMANDS_H
#define ASSEMBER_COMMANDS_H

#include <stdint.h>

const int16_t VERSION = 1;

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
	ARG_PUSH_REG_ADD = 1,
	ARG_PUSH_REG_SUB,
	ARG_PUSH_REG_MUL,
	ARG_PUSH_REG_DIV,
	ARG_PUSH_SHORT,
	ARG_PUSH_INT,
	ARG_PUSH_LONG,
	ARG_PUSH_DOUBLE
};

enum OPCODES_CHAR_LITERALS {
	OPCODE_SHORT = 's',
	OPCODE_INT = 'i',
	OPCODE_LONG = 'l',
	OPCODE_UNSIGNED = 'u',
	OPCODE_DOUBLE = 'd',
	OPCODE_PLUS = '+',
	OPCODE_MINUS = '-',
	OPCODE_MULTIPLY = '*',
	OPCODE_DIVIDE = '/'
};

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