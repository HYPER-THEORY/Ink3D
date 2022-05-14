/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, slicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, sject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or sstantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GL_MATH_INL
#define GL_MATH_INL

#include "gl_math.hpp"
#include "gl_stdinc.hpp"

namespace gl {

inline float random01f() {
	return rand() / (RAND_MAX + 1.f);
}

inline vec2f operator+(const vec2f& v1, const vec2f& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

inline vec2f operator-(const vec2f& v1, const vec2f& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

inline vec2f operator*(const vec2f& v1, float v2) {
	return {v1.x * v2, v1.y * v2};
}

inline vec2f operator*(float v1, const vec2f& v2) {
	return {v2.x * v1, v2.y * v1};
}

inline vec2f operator/(const vec2f& v1, float v2) {
	return {v1.x / v2, v1.y / v2};
}

inline float operator*(const vec2f& v1, const vec2f& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline float operator^(const vec2f& v1, const vec2f& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

inline vec3f operator+(const vec3f& v1, const vec3f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline vec3f operator-(const vec3f& v1, const vec3f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline vec3f operator*(const vec3f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

inline vec3f operator*(float v1, const vec3f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

inline vec3f operator/(const vec3f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

inline float operator*(const vec3f& v1, const vec3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3f operator^(const vec3f& v1, const vec3f& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

inline vec4f operator+(const vec4f& v1, const vec4f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline vec4f operator-(const vec4f& v1, const vec4f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

inline vec4f operator*(const vec4f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

inline vec4f operator*(float v1, const vec4f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

inline vec4f operator/(const vec4f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

inline float operator*(const vec4f& v1, const vec4f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template <int r, int c>
matf<r, c> operator+(const matf<r, c>& v1, const matf<r, c>& v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator-(const matf<r, c>& v1, const matf<r, c>& v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator*(const matf<r, c>& v1, float v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator*(float v1, const matf<r, c>& v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
matf<l1, l3> operator*(const matf<l1, l2>& v1, const matf<l2, l3>& v2) {
	matf<l1, l3> matrix;
	std::fill_n(matrix[0], l1 * l3, 0);
	for (int i = 0; i < l1; ++i) {
		for (int j = 0; j < l2; ++j) {
			for (int k = 0; k < l3; ++k) {
				matrix[i][k] += v1[i][j] * v2[j][k];
			}
		}
	}
	return matrix;
}

template <int r>
matf<r, 1> operator*(const matf<r, 2>& v1, const vec2f& v2) {
	matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
matf<r, 1> operator*(const matf<r, 3>& v1, const vec3f& v2) {
	matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
matf<r, 1> operator*(const matf<r, 4>& v1, const vec4f& v2) {
	matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator/(const matf<r, c>& v1, float v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}

inline float mat2x2_determinant(const matf<2, 2>& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

inline float mat3x3_determinant(const matf<3, 3>& m) {
	float inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2;
}

inline float mat4x4_determinant(const matf<4, 4>& m) {
	float s00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float s01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float s02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float s03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float s04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float s05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02) -
		m[0][1] * (m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04) +
		m[0][2] * (m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05) -
		m[0][3] * (m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05);
}

inline matf<2, 2> mat2x2_inverse(const matf<2, 2>& m) {
	float inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

inline matf<3, 3> mat3x3_inverse(const matf<3, 3>& m) {
	float inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	float inv_det = 1 / (m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2);
	return {
		inv_det * inv0,
		inv_det * (m[0][2] * m[2][1] - m[0][1] * m[2][2]),
		inv_det * (m[0][1] * m[1][2] - m[0][2] * m[1][1]),
		inv_det * inv1,
		inv_det * (m[0][0] * m[2][2] - m[0][2] * m[2][0]),
		inv_det * (m[1][0] * m[0][2] - m[0][0] * m[1][2]),
		inv_det * inv2,
		inv_det * (m[2][0] * m[0][1] - m[0][0] * m[2][1]),
		inv_det * (m[0][0] * m[1][1] - m[1][0] * m[0][1]),
	};
}

inline matf<4, 4> mat4x4_inverse(const matf<4, 4>& m) {
	float s00 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	float s01 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
	float s02 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
	float s03 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
	float s04 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
	float s05 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
	float s06 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	float s07 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
	float s08 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
	float s09 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	float s10 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
	float s11 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	float s12 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
	float s13 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
	float s14 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
	float s15 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
	float s16 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
	float s17 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	float inv0 = m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02;
	float inv1 = m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04;
	float inv2 = m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05;
	float inv3 = m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05;
	float inv_det = 1 / (m[0][0] * inv0 - m[0][1] * inv1 + m[0][2] * inv2 - m[0][3] * inv3);
	return {
		 inv_det * inv0,
		-inv_det * (m[0][1] * s00 - m[0][2] * s01 + m[0][3] * s02),
		 inv_det * (m[0][1] * s06 - m[0][2] * s07 + m[0][3] * s08),
		-inv_det * (m[0][1] * s09 - m[0][2] * s10 + m[0][3] * s11),
		-inv_det * inv1,
		 inv_det * (m[0][0] * s00 - m[0][2] * s03 + m[0][3] * s04),
		-inv_det * (m[0][0] * s06 - m[0][2] * s12 + m[0][3] * s13),
		 inv_det * (m[0][0] * s09 - m[0][2] * s14 + m[0][3] * s15),
		 inv_det * inv2,
		-inv_det * (m[0][0] * s01 - m[0][1] * s03 + m[0][3] * s05),
		 inv_det * (m[0][0] * s07 - m[0][1] * s12 + m[0][3] * s16),
		-inv_det * (m[0][0] * s10 - m[0][1] * s14 + m[0][3] * s17),
		-inv_det * inv3,
		 inv_det * (m[0][0] * s02 - m[0][1] * s04 + m[0][2] * s05),
		-inv_det * (m[0][0] * s08 - m[0][1] * s13 + m[0][2] * s16),
		 inv_det * (m[0][0] * s11 - m[0][1] * s15 + m[0][2] * s17),
	};
}

inline double random01d() {
	return rand() / (RAND_MAX + 1.);
}

inline vec2d operator+(const vec2d& v1, const vec2d& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

inline vec2d operator-(const vec2d& v1, const vec2d& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

inline vec2d operator*(const vec2d& v1, double v2) {
	return {v1.x * v2, v1.y * v2};
}

inline vec2d operator*(double v1, const vec2d& v2) {
	return {v2.x * v1, v2.y * v1};
}

inline vec2d operator/(const vec2d& v1, double v2) {
	return {v1.x / v2, v1.y / v2};
}

inline double operator*(const vec2d& v1, const vec2d& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline double operator^(const vec2d& v1, const vec2d& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

inline vec3d operator+(const vec3d& v1, const vec3d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline vec3d operator-(const vec3d& v1, const vec3d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline vec3d operator*(const vec3d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

inline vec3d operator*(double v1, const vec3d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

inline vec3d operator/(const vec3d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

inline double operator*(const vec3d& v1, const vec3d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3d operator^(const vec3d& v1, const vec3d& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

inline vec4d operator+(const vec4d& v1, const vec4d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline vec4d operator-(const vec4d& v1, const vec4d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

inline vec4d operator*(const vec4d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

inline vec4d operator*(double v1, const vec4d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

inline vec4d operator/(const vec4d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

inline double operator*(const vec4d& v1, const vec4d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template <int r, int c>
matd<r, c> operator+(const matd<r, c>& v1, const matd<r, c>& v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator-(const matd<r, c>& v1, const matd<r, c>& v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator*(const matd<r, c>& v1, double v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator*(double v1, const matd<r, c>& v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
matd<l1, l3> operator*(const matd<l1, l2>& v1, const matd<l2, l3>& v2) {
	matd<l1, l3> matrix;
	std::fill_n(matrix[0], l1 * l3, 0);
	for (int i = 0; i < l1; ++i) {
		for (int j = 0; j < l2; ++j) {
			for (int k = 0; k < l3; ++k) {
				matrix[i][k] += v1[i][j] * v2[j][k];
			}
		}
	}
	return matrix;
}

template <int r>
matd<r, 1> operator*(const matd<r, 2>& v1, const vec2d& v2) {
	matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
matd<r, 1> operator*(const matd<r, 3>& v1, const vec3d& v2) {
	matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
matd<r, 1> operator*(const matd<r, 4>& v1, const vec4d& v2) {
	matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator/(const matd<r, c>& v1, double v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}

inline double mat2x2_determinant(const matd<2, 2>& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

inline double mat3x3_determinant(const matd<3, 3>& m) {
	double inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2;
}

inline double mat4x4_determinant(const matd<4, 4>& m) {
	double s00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	double s01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	double s02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	double s03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	double s04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	double s05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02) -
		m[0][1] * (m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04) +
		m[0][2] * (m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05) -
		m[0][3] * (m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05);
}

inline matd<2, 2> mat2x2_inverse(const matd<2, 2>& m) {
	double inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

inline matd<3, 3> mat3x3_inverse(const matd<3, 3>& m) {
	double inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	double inv_det = 1 / (m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2);
	return {
		inv_det * inv0,
		inv_det * (m[0][2] * m[2][1] - m[0][1] * m[2][2]),
		inv_det * (m[0][1] * m[1][2] - m[0][2] * m[1][1]),
		inv_det * inv1,
		inv_det * (m[0][0] * m[2][2] - m[0][2] * m[2][0]),
		inv_det * (m[1][0] * m[0][2] - m[0][0] * m[1][2]),
		inv_det * inv2,
		inv_det * (m[2][0] * m[0][1] - m[0][0] * m[2][1]),
		inv_det * (m[0][0] * m[1][1] - m[1][0] * m[0][1]),
	};
}

inline matd<4, 4> mat4x4_inverse(const matd<4, 4>& m) {
	double s00 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	double s01 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
	double s02 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
	double s03 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
	double s04 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
	double s05 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
	double s06 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	double s07 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
	double s08 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
	double s09 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	double s10 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
	double s11 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	double s12 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
	double s13 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
	double s14 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
	double s15 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
	double s16 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
	double s17 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	double inv0 = m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02;
	double inv1 = m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04;
	double inv2 = m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05;
	double inv3 = m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05;
	double inv_det = 1 / (m[0][0] * inv0 - m[0][1] * inv1 + m[0][2] * inv2 - m[0][3] * inv3);
	return {
		 inv_det * inv0,
		-inv_det * (m[0][1] * s00 - m[0][2] * s01 + m[0][3] * s02),
		 inv_det * (m[0][1] * s06 - m[0][2] * s07 + m[0][3] * s08),
		-inv_det * (m[0][1] * s09 - m[0][2] * s10 + m[0][3] * s11),
		-inv_det * inv1,
		 inv_det * (m[0][0] * s00 - m[0][2] * s03 + m[0][3] * s04),
		-inv_det * (m[0][0] * s06 - m[0][2] * s12 + m[0][3] * s13),
		 inv_det * (m[0][0] * s09 - m[0][2] * s14 + m[0][3] * s15),
		 inv_det * inv2,
		-inv_det * (m[0][0] * s01 - m[0][1] * s03 + m[0][3] * s05),
		 inv_det * (m[0][0] * s07 - m[0][1] * s12 + m[0][3] * s16),
		-inv_det * (m[0][0] * s10 - m[0][1] * s14 + m[0][3] * s17),
		-inv_det * inv3,
		 inv_det * (m[0][0] * s02 - m[0][1] * s04 + m[0][2] * s05),
		-inv_det * (m[0][0] * s08 - m[0][1] * s13 + m[0][2] * s16),
		 inv_det * (m[0][0] * s11 - m[0][1] * s15 + m[0][2] * s17),
	};
}

}

#endif
