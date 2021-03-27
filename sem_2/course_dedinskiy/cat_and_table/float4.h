#ifndef INTR_FLOAT4
#define INTR_FLOAT4

union int4;

union float4 {
    __m128 m;
    float f[4];

    float4(__m128 val);
    float4(float  val);
    float4(float  val1, float val2, float val3, float val4);
    float4(const  int4& val);
    
    operator __m128() const;
    operator int4() const;

    float& operator[] (size_t n);
};

inline float4::float4 (__m128 val):
m(val)
{}

inline float4::float4 (float  val):
m(_mm_set_ps1(val))
{}

inline float4::float4(float  val1, float val2, float val3, float val4):
m(_mm_set_ps(val1, val2, val3, val4))
{}

inline float4::float4(const int4& val):
m(_mm_cvtepi32_ps(val))
{}

inline float4::operator __m128() const {
    return m;
}

inline float4::operator int4() const {
    return int4(_mm_castps_si128 (m));
}

inline float& float4::operator[](size_t n) {
    return f[n];
}

//-------------------------------------------------------------------------------------------------

inline float4 operator+(const float4& a, const float4& b) {
    return _mm_add_ps(a, b);
}

inline float4 operator-(const float4& a, const float4& b) {
    return _mm_sub_ps(a, b);
}

inline float4 operator*(const float4& a, const float4& b) {
    return _mm_mul_ps(a, b);
}

inline float4 operator/(const float4& a, const float4& b) {
    return _mm_div_ps(a, b);
}

inline float4 operator<=(const float4& a, const float4& b) {
    return _mm_cmple_ps(a, b);
}

inline int operator+(const float4& cmp) {
    return _mm_movemask_ps(cmp);
}

#endif // INTR_FLOAT4