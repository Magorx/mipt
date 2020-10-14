#ifndef METAINF_H
#define METAINF_H

#include <stdint.h>

typedef struct Signature_t {
	int32_t magic;
	int16_t version;
	int32_t file_size;
} Signature;

#endif // METAINF_H