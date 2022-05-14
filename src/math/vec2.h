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

#include <cmath>
#include <string>
#include <sstream>

#include "random.h"

class fvec2 {
public:
	float x = 0;
	float y = 0;
	
	fvec2() = default;
	
	fvec2(float x);
	
	fvec2(float x, float y);
	
	fvec2(const fvec2& v);
	
	fvec2 operator-() const;
	
	void operator+=(const fvec2& v);
	
	void operator-=(const fvec2& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const fvec2& v) const;
	
	fvec2 normalize() const;
	
	fvec2 rotate(float a) const;
	
	std::string to_string(int p = 2) const;
	
	static fvec2 random();
};

fvec2 operator+(const fvec2& v1, const fvec2& v2);

fvec2 operator-(const fvec2& v1, const fvec2& v2);

fvec2 operator*(const fvec2& v1, float v2);

fvec2 operator*(float v1, const fvec2& v2);

fvec2 operator/(const fvec2& v1, float v2);

float operator*(const fvec2& v1, const fvec2& v2);

float operator^(const fvec2& v1, const fvec2& v2);

using vec2 = fvec2;

class dvec2 {
public:
	double x = 0;
	double y = 0;
	
	dvec2() = default;
	
	dvec2(double x);
	
	dvec2(double x, double y);
	
	dvec2(const dvec2& v);
	
	dvec2 operator-() const;
	
	void operator+=(const dvec2& v);
	
	void operator-=(const dvec2& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const dvec2& v) const;
	
	dvec2 normalize() const;
	
	dvec2 rotate(double a) const;
	
	std::string to_string(int p = 2) const;
	
	static dvec2 randomunit();
};

dvec2 operator+(const dvec2& v1, const dvec2& v2);

dvec2 operator-(const dvec2& v1, const dvec2& v2);

dvec2 operator*(const dvec2& v1, double v2);

dvec2 operator*(double v1, const dvec2& v2);

dvec2 operator/(const dvec2& v1, double v2);

double operator*(const dvec2& v1, const dvec2& v2);

double operator^(const dvec2& v1, const dvec2& v2);
