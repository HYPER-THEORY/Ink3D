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

#pragma once

#include "vec3.h"

class fvec4 {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
	
	fvec4() = default;
	
	fvec4(float x);
	
	fvec4(float x, float y, float z, float w);
	
	fvec4(const fvec2& v, float z, float w);
	
	fvec4(float x, const fvec2& v, float w);
	
	fvec4(float x, float y, const fvec2& v);
	
	fvec4(const fvec2& v1, const fvec2& v2);
	
	fvec4(const fvec3& v, float w);
	
	fvec4(float x, const fvec3& v);
	
	fvec4(const fvec4& v);
	
	fvec4 operator-() const;
	
	void operator+=(const fvec4& v);
	
	void operator-=(const fvec4& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const fvec4& v) const;
	
	fvec4 normalize() const;
	
	std::string to_string(int p = 2) const;
};

fvec4 operator+(const fvec4& v1, const fvec4& v2);

fvec4 operator-(const fvec4& v1, const fvec4& v2);

fvec4 operator*(const fvec4& v1, float v2);

fvec4 operator*(float v1, const fvec4& v2);

fvec4 operator/(const fvec4& v1, float v2);

float operator*(const fvec4& v1, const fvec4& v2);

using vec4 = fvec4;

class dvec4 {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	double w = 0;
	
	dvec4() = default;
	
	dvec4(double x);
	
	dvec4(double x, double y, double z, double w);
	
	dvec4(const dvec2& v, double z, double w);
	
	dvec4(double x, const dvec2& v, double w);
	
	dvec4(double x, double y, const dvec2& v);
	
	dvec4(const dvec2& v1, const dvec2& v2);
	
	dvec4(const dvec3& v, double w);
	
	dvec4(double x, const dvec3& v);
	
	dvec4(const dvec4& v);
	
	dvec4 operator-() const;
	
	void operator+=(const dvec4& v);
	
	void operator-=(const dvec4& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const dvec4& v) const;
	
	dvec4 normalize() const;
	
	std::string to_string(int p = 2) const;
};

dvec4 operator+(const dvec4& v1, const dvec4& v2);

dvec4 operator-(const dvec4& v1, const dvec4& v2);

dvec4 operator*(const dvec4& v1, double v2);

dvec4 operator*(double v1, const dvec4& v2);

dvec4 operator/(const dvec4& v1, double v2);

double operator*(const dvec4& v1, const dvec4& v2);
