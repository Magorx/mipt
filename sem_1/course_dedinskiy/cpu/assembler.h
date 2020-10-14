#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>

const int32_t KCTF_MAGIC = 'FTCK';

int assemble_file(const char* fin_name, const char* fout_name);

#endif // ASSEMBLER_H