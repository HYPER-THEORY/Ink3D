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

#include "Vector3.h"

namespace Ink {

class FVec4 {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
	
	FVec4() = default;
	
	FVec4(float x);
	
	FVec4(float x, float y, float z, float w);
	
	FVec4(const FVec2& v, float z, float w);
	
	FVec4(float x, const FVec2& v, float w);
	
	FVec4(float x, float y, const FVec2& v);
	
	FVec4(const FVec2& v1, const FVec2& v2);
	
	FVec4(const FVec3& v, float w);
	
	FVec4(float x, const FVec3& v);
	
	FVec4 operator-() const;
	
	bool operator==(const FVec4& v) const;
	
	void operator+=(float v);
	
	void operator+=(const FVec4& v);
	
	void operator-=(float v);
	
	void operator-=(const FVec4& v);
	
	void operator*=(float v);
	
	void operator*=(const FVec4& v);
	
	void operator/=(float v);
	
	void operator/=(const FVec4& v);
	
	float dot(const FVec4& v) const;
	
	float magnitude() const;
	
	float distance(const FVec4& v) const;
	
	FVec4 normalize() const;
	
	std::string to_string(int p = 2) const;
};

FVec4 operator+(const FVec4& v1, float v2);

FVec4 operator+(float v1, const FVec4& v2);

FVec4 operator+(const FVec4& v1, const FVec4& v2);

FVec4 operator-(const FVec4& v1, float v2);

FVec4 operator-(float v1, const FVec4& v2);

FVec4 operator-(const FVec4& v1, const FVec4& v2);

FVec4 operator*(const FVec4& v1, float v2);

FVec4 operator*(float v1, const FVec4& v2);

FVec4 operator*(const FVec4& v1, const FVec4& v2);

FVec4 operator/(const FVec4& v1, float v2);

FVec4 operator/(float v1, const FVec4& v2);

FVec4 operator/(const FVec4& v1, const FVec4& v2);

using Vec4 = FVec4;

class DVec4 {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	double w = 0;
	
	DVec4() = default;
	
	DVec4(double x);
	
	DVec4(double x, double y, double z, double w);
	
	DVec4(const DVec2& v, double z, double w);
	
	DVec4(double x, const DVec2& v, double w);
	
	DVec4(double x, double y, const DVec2& v);
	
	DVec4(const DVec2& v1, const DVec2& v2);
	
	DVec4(const DVec3& v, double w);
	
	DVec4(double x, const DVec3& v);
	
	DVec4 operator-() const;
	
	bool operator==(const DVec4& v) const;
	
	void operator+=(double v);
	
	void operator+=(const DVec4& v);
	
	void operator-=(double v);
	
	void operator-=(const DVec4& v);
	
	void operator*=(double v);
	
	void operator*=(const DVec4& v);
	
	void operator/=(double v);
	
	void operator/=(const DVec4& v);
	
	double dot(const DVec4& v) const;
	
	double magnitude() const;
	
	double distance(const DVec4& v) const;
	
	DVec4 normalize() const;
	
	std::string to_string(int p = 2) const;
};

DVec4 operator+(const DVec4& v1, double v2);

DVec4 operator+(double v1, const DVec4& v2);

DVec4 operator+(const DVec4& v1, const DVec4& v2);

DVec4 operator-(const DVec4& v1, double v2);

DVec4 operator-(double v1, const DVec4& v2);

DVec4 operator-(const DVec4& v1, const DVec4& v2);

DVec4 operator*(const DVec4& v1, double v2);

DVec4 operator*(double v1, const DVec4& v2);

DVec4 operator*(const DVec4& v1, const DVec4& v2);

DVec4 operator/(const DVec4& v1, double v2);

DVec4 operator/(double v1, const DVec4& v2);

DVec4 operator/(const DVec4& v1, const DVec4& v2);

}
