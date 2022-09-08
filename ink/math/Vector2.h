/**
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

#include <string>

#include "Constants.h"
#include "Random.h"

namespace Ink {

class FVec2 {
public:
	float x = 0;
	float y = 0;
	
	FVec2(float x = 0);
	
	FVec2(float x, float y);
	
	FVec2 operator-() const;
	
	bool operator==(const FVec2& v) const;
	
	void operator+=(float v);
	
	void operator+=(const FVec2& v);
	
	void operator-=(float v);
	
	void operator-=(const FVec2& v);
	
	void operator*=(float v);
	
	void operator*=(const FVec2& v);
	
	void operator/=(float v);
	
	void operator/=(const FVec2& v);
	
	float dot(const FVec2& v) const;
	
	float cross(const FVec2& v) const;
	
	float magnitude() const;
	
	float distance(const FVec2& v) const;
	
	FVec2 normalize() const;
	
	FVec2 rotate(float a) const;
	
	std::string to_string(int p = 2) const;
	
	static FVec2 random();
};

FVec2 operator+(const FVec2& v1, float v2);

FVec2 operator+(float v1, const FVec2& v2);

FVec2 operator+(const FVec2& v1, const FVec2& v2);

FVec2 operator-(const FVec2& v1, float v2);

FVec2 operator-(float v1, const FVec2& v2);

FVec2 operator-(const FVec2& v1, const FVec2& v2);

FVec2 operator*(const FVec2& v1, float v2);

FVec2 operator*(float v1, const FVec2& v2);

FVec2 operator*(const FVec2& v1, const FVec2& v2);

FVec2 operator/(const FVec2& v1, float v2);

FVec2 operator/(float v1, const FVec2& v2);

FVec2 operator/(const FVec2& v1, const FVec2& v2);

using Vec2 = FVec2;

class DVec2 {
public:
	double x = 0;
	double y = 0;
	
	DVec2(double x = 0);
	
	DVec2(double x, double y);
	
	DVec2 operator-() const;
	
	bool operator==(const DVec2& v) const;
	
	void operator+=(double v);
	
	void operator+=(const DVec2& v);
	
	void operator-=(double v);
	
	void operator-=(const DVec2& v);
	
	void operator*=(double v);
	
	void operator*=(const DVec2& v);
	
	void operator/=(double v);
	
	void operator/=(const DVec2& v);
	
	double dot(const DVec2& v) const;
	
	double cross(const DVec2& v) const;
	
	double magnitude() const;
	
	double distance(const DVec2& v) const;
	
	DVec2 normalize() const;
	
	DVec2 rotate(double a) const;
	
	std::string to_string(int p = 2) const;
	
	static DVec2 random();
};

DVec2 operator+(const DVec2& v1, double v2);

DVec2 operator+(double v1, const DVec2& v2);

DVec2 operator+(const DVec2& v1, const DVec2& v2);

DVec2 operator-(const DVec2& v1, double v2);

DVec2 operator-(double v1, const DVec2& v2);

DVec2 operator-(const DVec2& v1, const DVec2& v2);

DVec2 operator*(const DVec2& v1, double v2);

DVec2 operator*(double v1, const DVec2& v2);

DVec2 operator*(const DVec2& v1, const DVec2& v2);

DVec2 operator/(const DVec2& v1, double v2);

DVec2 operator/(double v1, const DVec2& v2);

DVec2 operator/(const DVec2& v1, const DVec2& v2);

}
