#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "assembler.h"
#include "general.h"
#include "opcodes.h"
#include "metainf.h"

int assemble_file(const char *fin_name, const char* fout_name) {
	File fin = {};
    int ret = File_construct(&fin, fin_name);
    if (ret < 0) {
    	printf("let's go die\n");
        File_destruct(&fin);
        return -1;
    }

    Signature signature = {};
    signature.magic = KCTF_MAGIC;
    signature.version = VERSION;

    ByteOP *bop = new_ByteOP(STANDART_INIT_SIZE);

    ByteOP_put(bop, &signature, sizeof(Signature));

    printf("Lines in file: %zu\n", fin.lines_cnt);

    for (size_t line_i = 0; line_i < fin.lines_cnt; ++line_i) {
    	Line *line = fin.lines[line_i];
    	const unsigned char *str = line->string;

    	printf("[%zu] %s\n", line_i, str);

    	if (Line_starts_with(str, "push")) {
    		ByteOP_put_byte(bop, OPCODE_PUSH);

    		Line_verify_min_len(line, 8);

    		switch(str[5]) {
    			case OPCODE_SHORT:
    				ByteOP_put_byte(bop, OPCODE_SHORT);
    				break;
    			case OPCODE_INT:
    				ByteOP_put_int(bop, OPCODE_INT);
    				break;
    			case OPCODE_LONG:
    				ByteOP_put_long(bop, OPCODE_LONG);
    				break;
    			case OPCODE_UNSIGNED:
    				ByteOP_put_unsigned(bop, OPCODE_UNSIGNED);
    				break;
    			case OPCODE_DOUBLE:
    				ByteOP_put_byte(bop, OPCODE_DOUBLE);
    				break;
    			default:
    				printf("Line %zu has brocken args\n", line_i);
    		}

    		if (str[7] == 'r') {
    			Line_verify_min_len(line, 12);
    			const Byte register_index = (Byte) (str[6] - 'a');
    			switch (str[10]) {
    				case OPCODE_PLUS:
    					ByteOP_put_byte(bop, ARG_PUSH_REG_ADD);
    					break;
    				case OPCODE_MINUS:
    					ByteOP_put_byte(bop, ARG_PUSH_REG_SUB);
    					break;
    				case OPCODE_MULTIPLY:
    					ByteOP_put_byte(bop, ARG_PUSH_REG_MUL);
    					break;
    				case OPCODE_DIVIDE:
    					ByteOP_put_byte(bop, ARG_PUSH_REG_DIV);
    					break;
    				default:
    					printf("Line %zu has brocken args after reg\n", line_i);
    			}
    			double arg = 0;
    			if (!isdigit(str[11])) {
    				printf("Line %zu has brocken args after after reg\n", line_i);
    			}
    			sscanf((const char*) &str[11], "%lg", &arg);
    			ByteOP_put_byte(bop, register_index);
    			ByteOP_put_double(bop, arg);
    		} else {
    			if (!isdigit(str[7])) {
    				printf("Line %zu has brocken args\n", line_i);
    			}
    			double arg = 0;
    			sscanf((const char*) &str[7], "%lg", &arg);
    			ByteOP_put_double(bop, arg);
    		}
    	}
    }

    File_destruct(&fin);
    ByteOP_to_file(bop, fout_name);
    delete_ByteOP(bop);

	return 0;
}

int main() {
	assemble_file("prog.kc", "out");
	printf("OK\n");

	return 0;
}