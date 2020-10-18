#include "assembler.h"

// The assembler itself =======================================================

int main(const int argc, const char **argv) {
	const char *input_file  = "prog.kc";
	const char *output_file = "out.tf";
	if (argc > 1) {
		input_file = argv[1];
	}
	if (argc > 2) {
		output_file = argv[2];
	}

	assemble_file(input_file, output_file);

	return 0;
}

//=============================================================================

int check_and_process_opname(const unsigned char **symb, ByteOP *bop, const char *opname, const int opcode) {
	if (String_starts_with(*symb, opname)) {
		*symb += strlen(OPNAME_PUSH);
    	
    	Char_get_next_symb(symb);
    	
    	ByteOP_put_byte(bop, (byte) opcode);
    	return 1;
	}
	return 0;
}

int get_extract_register_index(const unsigned char **symb, byte *reg_index) {
	VERIFY(**symb == 'r');

	*symb += 1;

	const byte register_index = (byte) (**symb - 'a');
	VERIFY(register_index <= REGISTERS_COUNT);
	
	*symb += 2;
	Char_get_next_symb(symb);

	*reg_index = register_index;
	return 0;
}

int put_extract_register_index(const unsigned char **symb, ByteOP *bop) {
	byte reg_idx = 0;
	get_extract_register_index(symb, &reg_idx);
	ByteOP_put_byte(bop, reg_idx);
	return 0;
}

int get_extract_const_value(const unsigned char **symb, double *value) {
	double val = 0;
	int symbs_read = 0;
	sscanf((const char *) *symb, "%lg%n", &val, &symbs_read);
	VERIFY(symbs_read > 0);

	*symb += symbs_read;
	Char_get_next_symb(symb);

	*value = val;
	return 0;
}

int put_extract_operator_value(const unsigned char **symb, ByteOP *bop) {
	VERIFY(Char_in_string(**symb, OPERATIONS));
	ByteOP_put_byte(bop, **symb);

	*symb += 1;
	Char_get_next_symb(symb);

	return 0;
}

int put_extract_value(const unsigned char **symb, ByteOP *bop) {
	VERIFY(**symb != '\n');

	if (**symb == 'r') {
		byte reg_idx = 0;
		get_extract_register_index(symb, &reg_idx);
		if (Char_in_string(**symb, OPERATIONS)) {
			put_extract_operator_value(symb, bop);
			put_extract_value(symb, bop);
		}
		ByteOP_put_byte(bop, VALUE_REGISTER);
		ByteOP_put_byte(bop, reg_idx);
	} else {
		double const_val = 0;
		get_extract_const_value(symb, &const_val);
		if (Char_in_string(**symb, OPERATIONS)) {
			put_extract_operator_value(symb, bop);
			put_extract_value(symb, bop);
		}
		ByteOP_put_byte(bop, VALUE_CONSTANT);
		ByteOP_put_double(bop, const_val);
	}

	return 0;
}

int put_extract_line(const unsigned char **symb, ByteOP *bop) {
	for (int opcode = 0; opcode < (int) MAX_OPCODES_COUNT; ++opcode) {
		if (OPNAMES[opcode] == NULL) {
			continue;
		}

		if (check_and_process_opname(symb, bop, OPNAMES[opcode], opcode)) {
			 if (OPARGS[opcode]) {
			 	put_extract_value(symb, bop);
			 }
			 break;
		}
	}

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

    printf("------\n");

    for (size_t line_i = 0; line_i < fin.lines_cnt; ++line_i) {
    	Line *line = fin.lines[line_i];
    	const unsigned char *str = line->string;
    	const unsigned char *symb = str;
		Char_get_next_symb(&symb);

    	const int before_line_index = (int) (bop->cur_ptr - bop->buffer); // for listing

    	put_extract_line(&symb, bop);

    	// Listing =====

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

    	//==============
    }

    File_destruct(&fin);

    signature.file_size = bop->cur_ptr - bop->buffer;
    bop->cur_ptr = bop->buffer;
    ByteOP_put(bop, &signature, sizeof(Signature));
    bop->size -= sizeof(Signature);
    ByteOP_to_file(bop, fout_name);

    printf("------\n");

    printf("%.4ld - %s\n", signature.file_size, fout_name);

    delete_ByteOP(bop);

    printf("\n[END]<assembler>: Done assembling\n");

	return 0;
}
