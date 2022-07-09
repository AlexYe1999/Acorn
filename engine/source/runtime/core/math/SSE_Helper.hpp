#pragma once

#include<immintrin.h>
#include<xmmintrin.h>

#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)

// for row major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A1 |
//                                              | A2  A3 |
// 2x2 row major Matrix multiply A*B
inline __m128 Mat2Mul(__m128 vec1, __m128 vec2)
{
	return _mm_add_ps(
		_mm_mul_ps(vec1, VecSwizzle(vec2, 0,3,0,3)),
		_mm_mul_ps(VecSwizzle(vec1, 1, 0, 3, 2), VecSwizzle(vec2, 2, 1, 2, 1))
	);
}
// 2x2 row major Matrix adjugate multiply (A#)*B
inline __m128 Mat2AdjMul(__m128 vec1, __m128 vec2)
{
	return _mm_sub_ps(
		_mm_mul_ps(VecSwizzle(vec1, 3, 3, 0, 0), vec2),
		_mm_mul_ps(VecSwizzle(vec1, 1, 1, 2, 2), VecSwizzle(vec2, 2, 3, 0, 1))
	);

}
// 2x2 row major Matrix multiply adjugate A*(B#)
inline __m128 Mat2MulAdj(__m128 vec1, __m128 vec2)
{
	return _mm_sub_ps(
		_mm_mul_ps(vec1, VecSwizzle(vec2, 3, 0, 3, 0)),
		_mm_mul_ps(VecSwizzle(vec1, 1, 0, 3, 2), VecSwizzle(vec2, 2, 1, 2, 1))
	);
}

