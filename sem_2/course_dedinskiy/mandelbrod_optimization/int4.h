#ifndef INTR_INT4
#define INTR_INT4

union int4 {
    __m128i m;
    int i[4];

    int4(__m128i val);
    int4(int     val);
    int4(int     val1, int val2, int val3, int val4);

    operator __m128i() const;

    int& operator[](size_t n);
};

inline int4::int4(__m128i val):
m(val)
{}

inline int4::int4(int     val):
m(_mm_set1_epi32(val))
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

#endif // INTR_INT4