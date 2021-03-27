#ifndef INTR_INT4
#define INTR_INT4

union int4 {
    __m128i m;
    int i[4];

    int4(__m128i val);
    int4(const __m128i* data);
    int4(int     val);
    int4(int     val1, int val2, int val3, int val4);

    operator __m128i() const;

    int& operator[](size_t n);

    __m256i extend_zeroes_256_epi16();
    void mem_store(void *data);
};

inline int4::int4(__m128i val):
m(val)
{}

inline int4::int4(int     val):
m(_mm_set1_epi32(val))
{}

inline int4::int4(const __m128i* data):
m(_mm_load_si128(data))
{}

inline int4::int4(int val1, int val2, int val3, int val4):
m(_mm_set_epi32(val1, val2, val3, val4))
{}

inline int4::operator __m128i() const {
    return m;
}

inline int& int4::operator[] (size_t n) {
    return i[n];
}

inline int4 operator+(const int4& a, const int4& b) {
    return _mm_add_epi32(a, b);
}

inline int4 operator-(const int4& a, const int4& b) {
    return _mm_sub_epi32(a, b);
}

inline __m256i int4::extend_zeroes_256_epi16() {
    return _mm256_cvtepu8_epi16(m);
}

inline int4 join_lh(const int4 a, const int4 b) {
    return (__m128i) _mm_movelh_ps((__m128) a.m, (__m128) b.m);
}

inline void int4::mem_store(void *data) {
    _mm_storer_ps((float*) data, _mm_castsi128_ps(m));
}

#endif // INTR_INT4