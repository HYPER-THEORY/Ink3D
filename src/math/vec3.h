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

#include "vec2.h"

class fvec3 {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	
	fvec3() = default;
	
	fvec3(float x);
	
	fvec3(float x, float y, float z);
	
	fvec3(const fvec2& v, float z);
	
	fvec3(float x, const fvec2& v);
	
	fvec3(const fvec3& v);
	
	fvec3 operator-() const;
	
	void operator+=(const fvec3& v);
	
	void operator-=(const fvec3& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const fvec3& v) const;
	
	fvec3 normalize() const;
	
	fvec3 rotate(const fvec3& v, float a) const;
	
	std::string to_string(int p = 2) const;
	
	static fvec3 random();
};

fvec3 operator+(const fvec3& v1, const fvec3& v2);

fvec3 operator-(const fvec3& v1, const fvec3& v2);

fvec3 operator*(const fvec3& v1, float v2);

fvec3 operator*(float v1, const fvec3& v2);

fvec3 operator/(const fvec3& v1, float v2);

float operator*(const fvec3& v1, const fvec3& v2);

fvec3 operator^(const fvec3& v1, const fvec3& v2);

using vec3 = fvec3;

class dvec3 {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	
	dvec3() = default;
	
	dvec3(double x);
	
	dvec3(double x, double y, double z);
	
	dvec3(const dvec2& v, double z);
	
	dvec3(double x, const dvec2& v);
	
	dvec3(const dvec3& v);
	
	dvec3 operator-() const;
	
	void operator+=(const dvec3& v);
	
	void operator-=(const dvec3& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const dvec3& v) const;
	
	dvec3 normalize() const;
	
	dvec3 rotate(const dvec3& v, double a) const;
	
	std::string to_string(int p = 2) const;
	
	static dvec3 randomunit();
};

dvec3 operator+(const dvec3& v1, const dvec3& v2);

dvec3 operator-(const dvec3& v1, const dvec3& v2);

dvec3 operator*(const dvec3& v1, double v2);

dvec3 operator*(double v1, const dvec3& v2);

dvec3 operator/(const dvec3& v1, double v2);

double operator*(const dvec3& v1, const dvec3& v2);

dvec3 operator^(const dvec3& v1, const dvec3& v2);
