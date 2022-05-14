/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mat.h"

float mat2_determinant(const mat2& m) {
	return m[0][0] * m[1][1]
		 - m[0][1] * m[1][0];
}

float mat3_determinant(const mat3& m) {
	float sub0 = m[1][1] * m[2][2]
			   - m[2][1] * m[1][2];
	float sub1 = m[1][2] * m[2][0]
			   - m[1][0] * m[2][2];
	float sub2 = m[1][0] * m[2][1]
			   - m[2][0] * m[1][1];
	return m[0][0] * sub0
		 + m[0][1] * sub1
		 + m[0][2] * sub2;
}

float mat4_determinant(const mat4& m) {
	float sub0 = m[2][2] * m[3][3]
			   - m[3][2] * m[2][3];
	float sub1 = m[2][1] * m[3][3]
			   - m[3][1] * m[2][3];
	float sub2 = m[2][1] * m[3][2]
			   - m[3][1] * m[2][2];
	float sub3 = m[2][0] * m[3][3]
			   - m[3][0] * m[2][3];
	float sub4 = m[2][0] * m[3][2]
			   - m[3][0] * m[2][2];
	float sub5 = m[2][0] * m[3][1]
			   - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * sub0
					- m[1][2] * sub1
					+ m[1][3] * sub2) -
		   m[0][1] * (m[1][0] * sub0
					- m[1][2] * sub3
					+ m[1][3] * sub4) +
		   m[0][2] * (m[1][0] * sub1
					- m[1][1] * sub3
					+ m[1][3] * sub5) -
		   m[0][3] * (m[1][0] * sub2
					- m[1][1] * sub4
					+ m[1][2] * sub5);
}

mat2 mat2_inverse(const mat2& m) {
	float inv_det = 1 / (m[0][0] * m[1][1]
					   - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

mat3 mat3_inverse(const mat3& m) {
	float inv0 = m[1][1] * m[2][2]
			   - m[2][1] * m[1][2];
	float inv1 = m[1][2] * m[2][0]
			   - m[1][0] * m[2][2];
	float inv2 = m[1][0] * m[2][1]
			   - m[2][0] * m[1][1];
	float inv_det = 1 / (m[0][0] * inv0
					   + m[0][1] * inv1
					   + m[0][2] * inv2);
	return {
		inv_det * inv0,
		
		inv_det * (m[0][2] * m[2][1]
				 - m[0][1] * m[2][2]),
		inv_det * (m[0][1] * m[1][2]
				 - m[0][2] * m[1][1]),
		inv_det * inv1,
		
		inv_det * (m[0][0] * m[2][2]
				 - m[0][2] * m[2][0]),
		inv_det * (m[1][0] * m[0][2]
				 - m[0][0] * m[1][2]),
		inv_det * inv2,
		
		inv_det * (m[2][0] * m[0][1]
				 - m[0][0] * m[2][1]),
		inv_det * (m[0][0] * m[1][1]
				 - m[1][0] * m[0][1]),
	};
}

mat4 mat4_inverse(const mat4& m) {
	float sub00 = m[2][2] * m[3][3]
				- m[2][3] * m[3][2];
	float sub01 = m[2][1] * m[3][3]
				- m[2][3] * m[3][1];
	float sub02 = m[2][1] * m[3][2]
				- m[2][2] * m[3][1];
	float sub03 = m[2][0] * m[3][3]
				- m[2][3] * m[3][0];
	float sub04 = m[2][0] * m[3][2]
				- m[2][2] * m[3][0];
	float sub05 = m[2][0] * m[3][1]
				- m[2][1] * m[3][0];
	float sub06 = m[1][2] * m[3][3]
				- m[1][3] * m[3][2];
	float sub07 = m[1][1] * m[3][3]
				- m[1][3] * m[3][1];
	float sub08 = m[1][1] * m[3][2]
				- m[1][2] * m[3][1];
	float sub09 = m[1][2] * m[2][3]
				- m[1][3] * m[2][2];
	float sub10 = m[1][1] * m[2][3]
				- m[1][3] * m[2][1];
	float sub11 = m[1][1] * m[2][2]
				- m[1][2] * m[2][1];
	float sub12 = m[1][0] * m[3][3]
				- m[1][3] * m[3][0];
	float sub13 = m[1][0] * m[3][2]
				- m[1][2] * m[3][0];
	float sub14 = m[1][0] * m[2][3]
				- m[1][3] * m[2][0];
	float sub15 = m[1][0] * m[2][2]
				- m[1][2] * m[2][0];
	float sub16 = m[1][0] * m[3][1]
				- m[1][1] * m[3][0];
	float sub17 = m[1][0] * m[2][1]
				- m[1][1] * m[2][0];
	float inv00 = m[1][1] * sub00
				- m[1][2] * sub01
				+ m[1][3] * sub02;
	float inv01 = m[1][0] * sub00
				- m[1][2] * sub03
				+ m[1][3] * sub04;
	float inv02 = m[1][0] * sub01
				- m[1][1] * sub03
				+ m[1][3] * sub05;
	float inv03 = m[1][0] * sub02
				- m[1][1] * sub04
				+ m[1][2] * sub05;
	float inv_det = 1 / (m[0][0] * inv00
					   - m[0][1] * inv01
					   + m[0][2] * inv02
					   - m[0][3] * inv03);
	return {
		 inv_det * inv00,
		
		
		-inv_det * (m[0][1] * sub00
				  - m[0][2] * sub01
				  + m[0][3] * sub02),
		 inv_det * (m[0][1] * sub06
				  - m[0][2] * sub07
				  + m[0][3] * sub08),
		-inv_det * (m[0][1] * sub09
				  - m[0][2] * sub10
				  + m[0][3] * sub11),
		-inv_det * inv01,
		
		
		 inv_det * (m[0][0] * sub00
				  - m[0][2] * sub03
				  + m[0][3] * sub04),
		-inv_det * (m[0][0] * sub06
				  - m[0][2] * sub12
				  + m[0][3] * sub13),
		 inv_det * (m[0][0] * sub09
				  - m[0][2] * sub14
				  + m[0][3] * sub15),
		 inv_det * inv02,
		
		
		-inv_det * (m[0][0] * sub01
				  - m[0][1] * sub03
				  + m[0][3] * sub05),
		 inv_det * (m[0][0] * sub07
				  - m[0][1] * sub12
				  + m[0][3] * sub16),
		-inv_det * (m[0][0] * sub10
				  - m[0][1] * sub14
				  + m[0][3] * sub17),
		-inv_det * inv03,
		
		
		 inv_det * (m[0][0] * sub02
				  - m[0][1] * sub04
				  + m[0][2] * sub05),
		-inv_det * (m[0][0] * sub08
				  - m[0][1] * sub13
				  + m[0][2] * sub16),
		 inv_det * (m[0][0] * sub11
				  - m[0][1] * sub15
				  + m[0][2] * sub17),
	};
}

double mat2d_determinant(const dmat2& m) {
	return m[0][0] * m[1][1]
		 - m[0][1] * m[1][0];
}

double mat3d_determinant(const dmat3& m) {
	double sub0 = m[1][1] * m[2][2]
			    - m[2][1] * m[1][2];
	double sub1 = m[1][2] * m[2][0]
			    - m[1][0] * m[2][2];
	double sub2 = m[1][0] * m[2][1]
			    - m[2][0] * m[1][1];
	return m[0][0] * sub0
		 + m[0][1] * sub1
		 + m[0][2] * sub2;
}

double mat4d_determinant(const dmat4& m) {
	double sub0 = m[2][2] * m[3][3]
			    - m[3][2] * m[2][3];
	double sub1 = m[2][1] * m[3][3]
			    - m[3][1] * m[2][3];
	double sub2 = m[2][1] * m[3][2]
			    - m[3][1] * m[2][2];
	double sub3 = m[2][0] * m[3][3]
			    - m[3][0] * m[2][3];
	double sub4 = m[2][0] * m[3][2]
			    - m[3][0] * m[2][2];
	double sub5 = m[2][0] * m[3][1]
			    - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * sub0
					- m[1][2] * sub1
					+ m[1][3] * sub2) -
		   m[0][1] * (m[1][0] * sub0
					- m[1][2] * sub3
					+ m[1][3] * sub4) +
		   m[0][2] * (m[1][0] * sub1
					- m[1][1] * sub3
					+ m[1][3] * sub5) -
		   m[0][3] * (m[1][0] * sub2
					- m[1][1] * sub4
					+ m[1][2] * sub5);
}

dmat2 mat2d_inverse(const dmat2& m) {
	double inv_det = 1 / (m[0][0] * m[1][1]
					    - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

dmat3 mat3d_inverse(const dmat3& m) {
	double inv0 = m[1][1] * m[2][2]
			    - m[2][1] * m[1][2];
	double inv1 = m[1][2] * m[2][0]
			    - m[1][0] * m[2][2];
	double inv2 = m[1][0] * m[2][1]
			    - m[2][0] * m[1][1];
	double inv_det = 1 / (m[0][0] * inv0
					    + m[0][1] * inv1
					    + m[0][2] * inv2);
	return {
		inv_det * inv0,
		
		inv_det * (m[0][2] * m[2][1]
				 - m[0][1] * m[2][2]),
		inv_det * (m[0][1] * m[1][2]
				 - m[0][2] * m[1][1]),
		inv_det * inv1,
		
		inv_det * (m[0][0] * m[2][2]
				 - m[0][2] * m[2][0]),
		inv_det * (m[1][0] * m[0][2]
				 - m[0][0] * m[1][2]),
		inv_det * inv2,
		
		inv_det * (m[2][0] * m[0][1]
				 - m[0][0] * m[2][1]),
		inv_det * (m[0][0] * m[1][1]
				 - m[1][0] * m[0][1]),
	};
}

dmat4 mat4d_inverse(const dmat4& m) {
	double sub00 = m[2][2] * m[3][3]
				 - m[2][3] * m[3][2];
	double sub01 = m[2][1] * m[3][3]
				 - m[2][3] * m[3][1];
	double sub02 = m[2][1] * m[3][2]
				 - m[2][2] * m[3][1];
	double sub03 = m[2][0] * m[3][3]
				 - m[2][3] * m[3][0];
	double sub04 = m[2][0] * m[3][2]
				 - m[2][2] * m[3][0];
	double sub05 = m[2][0] * m[3][1]
				 - m[2][1] * m[3][0];
	double sub06 = m[1][2] * m[3][3]
				 - m[1][3] * m[3][2];
	double sub07 = m[1][1] * m[3][3]
				 - m[1][3] * m[3][1];
	double sub08 = m[1][1] * m[3][2]
				 - m[1][2] * m[3][1];
	double sub09 = m[1][2] * m[2][3]
				 - m[1][3] * m[2][2];
	double sub10 = m[1][1] * m[2][3]
				 - m[1][3] * m[2][1];
	double sub11 = m[1][1] * m[2][2]
				 - m[1][2] * m[2][1];
	double sub12 = m[1][0] * m[3][3]
				 - m[1][3] * m[3][0];
	double sub13 = m[1][0] * m[3][2]
				 - m[1][2] * m[3][0];
	double sub14 = m[1][0] * m[2][3]
				 - m[1][3] * m[2][0];
	double sub15 = m[1][0] * m[2][2]
				 - m[1][2] * m[2][0];
	double sub16 = m[1][0] * m[3][1]
				 - m[1][1] * m[3][0];
	double sub17 = m[1][0] * m[2][1]
				 - m[1][1] * m[2][0];
	double inv00 = m[1][1] * sub00
				 - m[1][2] * sub01
				 + m[1][3] * sub02;
	double inv01 = m[1][0] * sub00
				 - m[1][2] * sub03
				 + m[1][3] * sub04;
	double inv02 = m[1][0] * sub01
				 - m[1][1] * sub03
				 + m[1][3] * sub05;
	double inv03 = m[1][0] * sub02
				 - m[1][1] * sub04
				 + m[1][2] * sub05;
	double inv_det = 1 / (m[0][0] * inv00
						- m[0][1] * inv01
						+ m[0][2] * inv02
						- m[0][3] * inv03);
	return {
		 inv_det * inv00,
		
		
		-inv_det * (m[0][1] * sub00
				  - m[0][2] * sub01
				  + m[0][3] * sub02),
		 inv_det * (m[0][1] * sub06
				  - m[0][2] * sub07
				  + m[0][3] * sub08),
		-inv_det * (m[0][1] * sub09
				  - m[0][2] * sub10
				  + m[0][3] * sub11),
		-inv_det * inv01,
		
		
		 inv_det * (m[0][0] * sub00
				  - m[0][2] * sub03
				  + m[0][3] * sub04),
		-inv_det * (m[0][0] * sub06
				  - m[0][2] * sub12
				  + m[0][3] * sub13),
		 inv_det * (m[0][0] * sub09
				  - m[0][2] * sub14
				  + m[0][3] * sub15),
		 inv_det * inv02,
		
		
		-inv_det * (m[0][0] * sub01
				  - m[0][1] * sub03
				  + m[0][3] * sub05),
		 inv_det * (m[0][0] * sub07
				  - m[0][1] * sub12
				  + m[0][3] * sub16),
		-inv_det * (m[0][0] * sub10
				  - m[0][1] * sub14
				  + m[0][3] * sub17),
		-inv_det * inv03,
		
		
		 inv_det * (m[0][0] * sub02
				  - m[0][1] * sub04
				  + m[0][2] * sub05),
		-inv_det * (m[0][0] * sub08
				  - m[0][1] * sub13
				  + m[0][2] * sub16),
		 inv_det * (m[0][0] * sub11
				  - m[0][1] * sub15
				  + m[0][2] * sub17),
	};
}
