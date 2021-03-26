#ifndef INTRINSIC_TYPES
#define INTRINSIC_TYPES

#include <emmintrin.h>

union v_int4;

union v_float4 {
    __m128 m;
    float  f[4];

    v_float4 (__m128 val);
    v_float4 (float val);
    v_float4 (float val1, float val2, float val3, float val4);
    v_float4 (const v_int4& val);
    
    operator __m128() const;
    operator v_int4() const;

    float& operator[] (size_t n);
};

//-------------------------------------------------------------------------------------------------

union v_int4 {
    __m128i m;
    int     i[4];

    v_int4 (__m128i val);
    v_int4 (int val);
    v_int4 (int val1, int val2, int val3, int val4);

    operator __m128i() const;

    int& operator[] (size_t n);
};

//-------------------------------------------------------------------------------------------------

inline v_float4::v_float4 (__m128 val)                                      : m (val)                                      {}
inline v_float4::v_float4 (float  val)                                      : m (_mm_set_ps1     (val))                    {}
inline v_float4::v_float4 (float  val1, float val2, float val3, float val4) : m (_mm_set_ps      (val1, val2, val3, val4)) {}
inline v_float4::v_float4 (const v_int4& val)                               : m (_mm_cvtepi32_ps (val))                    {}

inline v_float4::operator __m128() const                                  { return m;                                     }
inline v_float4::operator v_int4() const                                  { return v_int4 (_mm_castps_si128 (m));         }
inline float& v_float4::operator[] (size_t n)                             { return f[n];                                  }

//-------------------------------------------------------------------------------------------------

inline v_int4::v_int4 (__m128i val)                                     : m (val)                                      {}
inline v_int4::v_int4 (int     val)                                     : m (_mm_set1_epi32  (val))                    {}
inline v_int4::v_int4 (int     val1, int val2, int val3, int val4)      : m (_mm_set_epi32   (val1, val2, val3, val4)) {}

inline v_int4::operator __m128i() const                                 { return m;                                     }
inline int& v_int4::operator[] (size_t n)                               { return i[n];                                  }

//-------------------------------------------------------------------------------------------------

inline v_float4 operator+  (const v_float4& a, const v_float4& b) { return _mm_add_ps      (a, b); }
inline v_float4 operator-  (const v_float4& a, const v_float4& b) { return _mm_sub_ps      (a, b); }
inline v_float4 operator*  (const v_float4& a, const v_float4& b) { return _mm_mul_ps      (a, b); }
inline v_float4 operator/  (const v_float4& a, const v_float4& b) { return _mm_div_ps      (a, b); }
inline v_float4 operator<= (const v_float4& a, const v_float4& b) { return _mm_cmple_ps    (a, b); }
inline int      operator+  (const v_float4& cmp)                  { return _mm_movemask_ps (cmp) ; }

inline v_float4 sqrt        (const v_float4& x)                   { return _mm_sqrt_ps     (x)   ; }

inline v_int4 operator +  (const v_int4& a, const v_int4& b) { return _mm_add_epi32   (a, b); }
inline v_int4 operator -  (const v_int4& a, const v_int4& b) { return _mm_sub_epi32   (a, b); }

#endif // INTRINSIC_TYPES