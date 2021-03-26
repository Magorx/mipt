#ifndef COLOR_PACK4
#define COLOR_PACK4

#include <emmintrin.h>

union ARGB{
	unsigned char argb[4];
	struct {
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	int i;

	ARGB():            i(0) {}
	ARGB(const int x): i(x) {}

	ARGB(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_ = 255):
	a(a_),
	r(r_),
	g(g_),
	b(b_)
	{}

	ARGB(const int r_, const int g_, const int b_, const int a_ = 255):
	a(a_),
	r(r_),
	g(g_),
	b(b_)
	{}

	operator int() const;
};

inline ARGB::operator int() const { return i; }
inline ARGB mult      (const float num, const ARGB argb) { return {(unsigned char) (argb.r * num),
																   (unsigned char) (argb.g * num),
																   (unsigned char) (argb.b * num),
																   (unsigned char) (argb.a      )}; }
inline ARGB operator+(const ARGB other, const ARGB argb) { return {(unsigned char) (argb.r + other.r),
																   (unsigned char) (argb.g + other.g),
																   (unsigned char) (argb.b + other.b),
																   (unsigned char) (argb.a          )}; }

union ColorP4 {
	__m128i m;
	ARGB argb[4];

	ColorP4 (__m128i val);
	ColorP4 (ARGB    val);
	ColorP4 (ARGB    val1, ARGB val2, ARGB val3, ARGB val4);

	operator __m128i() const;

	ARGB& operator[] (size_t n);
};

inline ColorP4::ColorP4 (__m128i val)                                     : m (val)                                      {}
inline ColorP4::ColorP4 (ARGB    val)                                     : m (_mm_set1_epi32  (val))                    {}
inline ColorP4::ColorP4 (ARGB    val1, ARGB val2, ARGB val3, ARGB val4)   : m (_mm_set_epi32   (val1, val2, val3, val4)) {}

inline ColorP4::operator __m128i() const                                  { return m;                                     }
inline ARGB &ColorP4::operator[] (size_t n)								  { return argb[n];                               }

inline ColorP4 operator+  (const ColorP4& a, const ColorP4& b) { return _mm_add_epi32   (a, b); }
inline ColorP4 operator-  (const ColorP4& a, const ColorP4& b) { return _mm_sub_epi32   (a, b); }

#endif // COLOR_PACK4