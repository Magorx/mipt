#ifndef METAINF_H
#define METAINF_H

#include <stdint.h>

typedef struct Signature_t {
	int32_t magic;
	int32_t version;
	int64_t file_size;
} Signature;

#endif // METAINF_H