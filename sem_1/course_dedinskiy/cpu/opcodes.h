#ifndef ASSEMBER_COMMANDS_H
#define ASSEMBER_COMMANDS_H

#include <stdint.h>

const int16_t VERSION = 2;

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
	1,
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



#endif