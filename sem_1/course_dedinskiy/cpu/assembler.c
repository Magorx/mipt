#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "assembler.h"
#include "general.h"
#include "opcodes.h"
#include "metainf.h"

int extract_register_index(const unsigned char **symb, Byte *reg_index) {
	VERIFY(**symb == 'r');
	*symb += 1;
	const Byte register_index = (Byte) (**symb - 'a');
	VERIFY(register_index <= REGISTERS_COUNT);
	*symb += 2;

	Char_get_next_symb(symb);

	*reg_index = register_index;
	return 0;
}

int extract_operation_code(const unsigned char **symb, Byte *opcode) {
	const Byte operation_code = **symb;
	*symb += 1;
	Char_get_next_symb(symb);

	*opcode = operation_code;
	return 0;
}

int extract_const_value(const unsigned char **symb, double *value) {
	double val = 0;
	int symbs_read = 0;
	sscanf((const char *) *symb, "%lg%n", &val, &symbs_read);
	VERIFY(symbs_read > 0);

	*value = val;
	return 0;
}

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

    printf("[SAT]<assembler>: Starting assembling\n");
    printf("[   ]<         >: [filename](%s) [lines_cnt](%zu)\n", fin_name, fin.lines_cnt);
    printf("\n");

    for (size_t line_i = 0; line_i < fin.lines_cnt; ++line_i) {
    	Line *line = fin.lines[line_i];
    	const unsigned char *str = line->string;
    	const unsigned char *symb = str;

    	const int before_line_index = (int) (bop->cur_ptr - bop->buffer); // for listing

    	if (Line_starts_with(symb, OPNAME_PUSH)) {
    		symb += strlen(OPNAME_PUSH);
    		Char_get_next_symb(&symb);

    		if (*symb == 'r') {
    			ByteOP_put_byte(bop, OPCODE_PUSH_REG);

    			Byte reg_idx = 0;
    			Byte operation = 0;
    			double const_val = 0;
    			
    			extract_register_index(&symb, &reg_idx);
    			extract_operation_code(&symb, &operation);
    			extract_const_value   (&symb, &const_val);

    			ByteOP_put_byte   (bop, reg_idx);
    			ByteOP_put_byte   (bop, operation);
    			ByteOP_put_double (bop, const_val);
    		} else {
    			ByteOP_put_byte(bop, OPCODE_PUSH_STACK);

    			double const_val = 0;
    			extract_const_value(&symb, &const_val);
    			ByteOP_put_double(bop, const_val);
    		}
    	}

    	printf("%.4ld | ", bop->cur_ptr - bop->buffer);
    	size_t bytes_prined = 0;
    	for (size_t i = 0; i < (size_t) (bop->cur_ptr - bop->buffer - before_line_index); ++i) {
    		printf("%.2x ", bop->buffer[before_line_index + (int) i]);
    		++bytes_prined;
    	}

    	while (bytes_prined < STANDART_BYTE_LINE_BYTES_COUNT) {
    		printf("   ");
    		++bytes_prined;
    	}
    	printf("| [%zu] %s", line_i, str);
    	printf("\n");

    }

    File_destruct(&fin);
    ByteOP_to_file(bop, fout_name);
    delete_ByteOP(bop);

    printf("\n[END]<assembler>: Done assembling\n");

	return 0;
}

typedef union Uni_t {
	int i;
	long l;
	double d;
} Uni;

int main() {
	assemble_file("prog.kc", "out.kctf");

	return 0;
}