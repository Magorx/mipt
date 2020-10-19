#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef uint8_t byte;

// ByteOP =====================================================================

typedef struct ByteOP_t {
    size_t capacity;
    size_t size;
    byte *buffer;
    byte *cur_ptr;
} ByteOP;

ByteOP *new_ByteOP(const size_t size) {
    ByteOP *bop = calloc(sizeof(ByteOP), 1);
    if (!bop) {
        return NULL;
    }

    bop->buffer = calloc(size, 1);
    if (!bop->buffer) {
        free(bop);
        return NULL;
    }

    bop->cur_ptr = bop->buffer;
    bop->capacity = size;
    bop->size = 0;

    return bop;
}

int delete_ByteOP(ByteOP *cake) {
    free(cake->buffer);

    cake->buffer = (byte*) KCTF_POISON;
    cake->cur_ptr = (byte*) KCTF_POISON;
    cake->capacity = (size_t) KCTF_POISON;
    cake->size = (size_t) KCTF_POISON;

    free(cake);

    return 0;
}

int ByteOP_realloc_up(ByteOP *cake) {
    assert(cake);
    void *new_ptr = realloc_buffer(cake->buffer, cake->capacity, 2);
    if (!new_ptr) {
        return ERROR_REALLOC_FAIL;
    }

    long int shift = cake->cur_ptr - cake->buffer;
    cake->buffer = (byte*) new_ptr;
    cake->cur_ptr = cake->buffer + shift;
    cake->capacity = cake->capacity * 2;

    return 0;
}

int ByteOP_put(ByteOP *cake, const void *src, const size_t size) {
    assert(cake != NULL);
    assert(src  != NULL);

    if (cake->size + size >= cake->capacity) {
        assert_OK(ByteOP_realloc_up(cake));
    }

    memcpy(cake->cur_ptr, src, size);
    cake->cur_ptr += size;
    cake->size += size;

    return 0;
}

int ByteOP_put_byte(ByteOP *cake, const byte src) {
    return ByteOP_put(cake, &src, sizeof(byte));
}

int ByteOP_put_int(ByteOP *cake, const int src) {
    return ByteOP_put(cake, &src, sizeof(int));
}

int ByteOP_put_long(ByteOP *cake, const long src) {
    return ByteOP_put(cake, &src, sizeof(long));
}

int ByteOP_put_unsigned(ByteOP *cake, const unsigned int src) {
    return ByteOP_put(cake, &src, sizeof(unsigned int));
}

int ByteOP_put_double(ByteOP *cake, const double src) {
    return ByteOP_put(cake, &src, sizeof(double));
}

int ByteOP_to_file(const ByteOP *cake, const char* filename) {
    assert(cake != NULL);
    FILE *fout = fopen(filename, "wb");
    fwrite(cake->buffer, sizeof(byte), cake->size, fout);
    fclose(fout);

    return 0;
}

//=============================================================================
// ByteIP =====================================================================

typedef struct ByteIP_t {
    size_t capacity;
    size_t cur_idx;
    size_t size;
    byte *buffer;
} ByteIP;

ByteIP *new_ByteIP(const size_t capacity) {
    ByteIP *bip = calloc(sizeof(ByteIP), 1);
    if (!bip) {
        return NULL;
    }

    bip->buffer = calloc(capacity + 1, 1);
    if (!bip->buffer) {
        free(bip);
        return NULL;
    }

    bip->capacity = capacity;
    bip->cur_idx = 0;

    return bip;
}

int delete_ByteIP(ByteIP *cake) {
    free(cake->buffer);

    cake->buffer = (byte*) KCTF_POISON;
    cake->capacity = (size_t) KCTF_POISON;
    cake->size = (size_t) KCTF_POISON;
    cake->cur_idx = (size_t) KCTF_POISON;

    free(cake);

    return 0;
}

int ByteIP_read_file(ByteIP *cake, const char *file_name, const size_t file_size) {
    assert(cake != NULL);
    assert(file_name != NULL);

    if (file_size > cake->capacity) {
        byte *new_buffer = realloc(cake->buffer, file_size + 1);
        assert(new_buffer != NULL);

        cake->buffer = new_buffer;
        cake->capacity = file_size;
    }

    int file_dscr = open(file_name, O_RDONLY);
    cake->size = (size_t) read(file_dscr, cake->buffer, cake->capacity);
    close(file_dscr);

    return 0;
}

int ByteIP_left_to_file(ByteIP *cake, const char *file_name) {
    assert(cake != NULL);

    FILE *fout = fopen(file_name, "wb");
    fwrite(&cake->buffer[cake->cur_idx], sizeof(byte), cake->size - cake->cur_idx, fout);
    fclose(fout);

    return 0;
}

int ByteIP_reset(ByteIP *cake) {
    assert(cake != NULL);

    cake->cur_idx = 0;
    return 0;
}

int ByteIP_get(ByteIP *cake, void *dest, const size_t size) {
    assert(cake != NULL);
    assert(dest != NULL);

    if (cake->cur_idx + size > cake->size) {
        return ERROR_ERROR;
    } else {
        memcpy(dest, &cake->buffer[cake->cur_idx], size);
        cake->cur_idx += size;
        return 0;
    }
}

int ByteIP_get_byte(ByteIP *cake, byte *dest) {
    return ByteIP_get(cake, dest, sizeof(byte));
}

int ByteIP_get_int(ByteIP *cake, int *dest) {
    return ByteIP_get(cake, dest, sizeof(int));
}

int ByteIP_get_long(ByteIP *cake, long *dest) {
    return ByteIP_get(cake, dest, sizeof(long));
}

int ByteIP_get_unsigned(ByteIP *cake, unsigned *dest) {
    return ByteIP_get(cake, dest, sizeof(unsigned));
}

int ByteIP_get_double(ByteIP *cake, double *dest) {
    return ByteIP_get(cake, dest, sizeof(double));
}


//=============================================================================
