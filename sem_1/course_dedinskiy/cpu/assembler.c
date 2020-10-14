#include <stdlib.h>
#include <stdio.h>

#include "assembler.h"
#include "general.h"
#include "opcodes.h"
#include "metainf.h"

int assemble_file(const char* fin_name, const char* fout_name) {
	File fin = {};
    int ret = File_construct(&fin, fin_name);
    if (ret < 0) {
    	printf("let's go die\n");
        File_destruct(&fin);
        return -1;
    }

    FILE *fout = fopen(fout_name, "wb");

    Signature signature = {};
    signature.magic = KCTF_MAGIC;
    signature.version = VERSION;

    assert(fwrite(&signature, sizeof(Signature), 1, fout) == 1);

    fclose(fout);
    File_destruct(&fin);

	return 0;
}

int main() {
	assemble_file("prog.kc", "prog.out");
	printf("OK\n");

	return 0;
}