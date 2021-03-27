#ifndef M256_H
#define M256_H

union m256 {
	__m256i mi;
	__m256  mf;
	__m256d md;

	int4    mint4[2];
	__m128  mf128[2];
	__m128d md128[2];

	int    i[8];
	float  f[8];
	double d[4];

	m256(const int val);
	m256(const int v1, const int v2, const int v3, const int v4);
	m256(const int v1, const int v2, const int v3, const int v4, 
		 const int v5, const int v6, const int v7, const int v8);

	m256(const __m256i val);
	m256(const int4 val);
	m256(const __m128i* data);

	m256 zero_extend_256_epi16();
	m256 shuffle_256_epi8(const m256 mask);

	m256 sub_256_epi16(const m256 other);
	m256 add_256_epi16(const m256 other);
	m256 mul_256_epi16(const m256 other);
};

inline m256::m256(const int val):
mi(_mm256_set1_epi32(val))
{}

inline m256::m256(const int v1, const int v2, const int v3, const int v4):
mint4({_mm_set_epi32(v1, v2, v3 ,v4), _mm_set_epi32(v1, v2, v3 ,v4)})
{}

inline m256::m256(const int4 val):
mint4({val, val})
{}

inline m256::m256(const int v1, const int v2, const int v3, const int v4, 
				  const int v5, const int v6, const int v7, const int v8):
mi(_mm256_set_epi32(v1, v2, v3 ,v4, v5, v6, v7, v8))
{}

inline m256::m256(const __m128i* data):
mint4({_mm_load_si128(data), mint4[0]})
{}

inline m256::m256(const __m256i val):
mi(val)
{}

//=============================================================================

inline m256 m256::shuffle_256_epi8(const m256 mask) {
	return _mm256_shuffle_epi8(mi, mask.mi);
}

inline m256 m256::sub_256_epi16(const m256 other) {
	return _mm256_sub_epi16(mi, other.mi);
}

inline m256 m256::add_256_epi16(const m256 other) {
	return _mm256_add_epi16(mi, other.mi);
}

inline m256 m256::mul_256_epi16(const m256 other) {
	return _mm256_mullo_epi16(mi, other.mi);
}

#endif // M256_H