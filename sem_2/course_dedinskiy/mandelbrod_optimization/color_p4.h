#ifndef COLOR_PACK4
#define COLOR_PACK4

inline get_int_byte(int number, int n) { return (number >> (8*n)) & 0xff; }

union ARGB {
	char argb[4];
	struct {
		char a;
		char r;
		char g;
		char b;
	}
	int i;

	ARGB(const int color) {
		i = color;
	}
};

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