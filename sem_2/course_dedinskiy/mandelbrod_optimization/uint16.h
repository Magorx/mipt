#ifndef INTR_UINT16
#define INTR_UINT16

typedef unsigned char uchr;

union uchr16 {
    __m128i m;
    uchr i[16];

    uchr16(__m128i val);
    uchr16(uchr val);
    uchr16(uchr val1,  uchr val2,  uchr val3,  uchr val4,
           uchr val5,  uchr val6,  uchr val7,  uchr val8,
           uchr val9,  uchr val10, uchr val11, uchr val12,
           uchr val13, uchr val14, uchr val15, uchr val16);

    operator __m128i() const;

    uchr& operator[](size_t n);
};

inline uchr16::uchr16(__m128i val):
m(val)
{}

inline uchr16::uchr16(uchr     val):
m(_mm_set1_epi8(val))
{}

inline uchr16::uchr16(uchr val1,  uchr val2,  uchr val3,  uchr val4,
                      uchr val5,  uchr val6,  uchr val7,  uchr val8,
                      uchr val9,  uchr val10, uchr val11, uchr val12,
                      uchr val13, uchr val14, uchr val15, uchr val16):
m(_mm_set_epi8(val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14, val15, val16))
{}

inline uchr16::operator __m128i() const {
    return m;
}

inline uchr& uchr16::operator[] (size_t n) {
    return i[n];
}

inline uchr16 operator+(const uchr16& a, const uchr16& b) {
    return _mm_add_epi8(a, b);
}

inline uchr16 operator-(const uchr16& a, const uchr16& b) {
    return _mm_sub_epi8(a, b);
}

#endif // INTR_UINT16