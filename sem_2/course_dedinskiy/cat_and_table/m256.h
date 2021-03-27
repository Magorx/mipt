#ifndef M256_H
#define M256_H

typedef unsigned char uchr;

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
	m256(const int16_t val);
	m256(const int v1, const int v2, const int v3, const int v4);
	m256(const int v1, const int v2, const int v3, const int v4, 
		 const int v5, const int v6, const int v7, const int v8);

	m256(const uchr v1 , const uchr v2 , const uchr v3 , const uchr v4 , 
		 const uchr v5 , const uchr v6 , const uchr v7 , const uchr v8 ,
		 const uchr v9 , const uchr v10, const uchr v11, const uchr v12, 
		 const uchr v13, const uchr v14, const uchr v15, const uchr v16,
		 const uchr v17, const uchr v18, const uchr v19, const uchr v20, 
		 const uchr v21, const uchr v22, const uchr v23, const uchr v24,
		 const uchr v25, const uchr v26, const uchr v27, const uchr v28, 
		 const uchr v29, const uchr v30, const uchr v31, const uchr v32);

	m256(const __m256i val);
	m256(const int4 val);

	m256 zero_extend_256_epi16();
	m256 shuffle_256_epi8(const m256 mask);

	m256 sub_256_epi16(const m256 other);
	m256 add_256_epi16(const m256 other);
	m256 mul_256_epi16(const m256 other);
};

inline m256::m256(const int val):
mi(_mm256_set1_epi32(val))
{}

inline m256::m256(const int16_t val):
mi(_mm256_set1_epi16(val))
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

inline m256::m256(const __m256i val):
mi(val)
{}

inline m256::m256(const uchr v1 , const uchr v2 , const uchr v3 , const uchr v4 , 
				  const uchr v5 , const uchr v6 , const uchr v7 , const uchr v8 ,
				  const uchr v9 , const uchr v10, const uchr v11, const uchr v12, 
				  const uchr v13, const uchr v14, const uchr v15, const uchr v16,
				  const uchr v17, const uchr v18, const uchr v19, const uchr v20, 
				  const uchr v21, const uchr v22, const uchr v23, const uchr v24,
				  const uchr v25, const uchr v26, const uchr v27, const uchr v28, 
				  const uchr v29, const uchr v30, const uchr v31, const uchr v32):
mi(_mm256_set_epi8(v1 , v2 , v3 , v4 , v5 , v6 , v7 , v8 ,
				   v9 , v10, v11, v12, v13, v14, v15, v16,
				   v17, v18, v19, v20, v21, v22, v23, v24,
				   v25, v26, v27, v28, v29, v30, v31, v32))
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