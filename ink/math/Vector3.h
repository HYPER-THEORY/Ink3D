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

#include "Vector2.h"

namespace Ink {

class FVec3 {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	
	FVec3(float x = 0);
	
	FVec3(float x, float y, float z);
	
	FVec3(const FVec2& v, float z);
	
	FVec3(float x, const FVec2& v);
	
	FVec3 operator-() const;
	
	bool operator==(const FVec3& v) const;
	
	void operator+=(float v);
		
	void operator+=(const FVec3& v);
	
	void operator-=(float v);
	
	void operator-=(const FVec3& v);
	
	void operator*=(float v);
	
	void operator*=(const FVec3& v);
	
	void operator/=(float v);
	
	void operator/=(const FVec3& v);
	
	float dot(const FVec3& v) const;
	
	FVec3 cross(const FVec3& v) const;
	
	float magnitude() const;
	
	float distance(const FVec3& v) const;
	
	FVec3 normalize() const;
	
	FVec3 rotate(const FVec3& v, float a) const;
	
	std::string to_string(int p = 2) const;
	
	static FVec3 random();
};

FVec3 operator+(const FVec3& v1, float v2);

FVec3 operator+(float v1, const FVec3& v2);

FVec3 operator+(const FVec3& v1, const FVec3& v2);

FVec3 operator-(const FVec3& v1, float v2);

FVec3 operator-(float v1, const FVec3& v2);

FVec3 operator-(const FVec3& v1, const FVec3& v2);

FVec3 operator*(const FVec3& v1, float v2);

FVec3 operator*(float v1, const FVec3& v2);

FVec3 operator*(const FVec3& v1, const FVec3& v2);

FVec3 operator/(const FVec3& v1, float v2);

FVec3 operator/(float v1, const FVec3& v2);

FVec3 operator/(const FVec3& v1, const FVec3& v2);

using Vec3 = FVec3;

class DVec3 {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	
	DVec3(double x = 0);
	
	DVec3(double x, double y, double z);
	
	DVec3(const DVec2& v, double z);
	
	DVec3(double x, const DVec2& v);
	
	DVec3 operator-() const;
	
	bool operator==(const DVec3& v) const;
	
	void operator+=(double v);
		
	void operator+=(const DVec3& v);
	
	void operator-=(double v);
	
	void operator-=(const DVec3& v);
	
	void operator*=(double v);
	
	void operator*=(const DVec3& v);
	
	void operator/=(double v);
	
	void operator/=(const DVec3& v);
	
	double dot(const DVec3& v) const;
	
	DVec3 cross(const DVec3& v) const;
	
	double magnitude() const;
	
	double distance(const DVec3& v) const;
	
	DVec3 normalize() const;
	
	DVec3 rotate(const DVec3& v, double a) const;
	
	std::string to_string(int p = 2) const;
	
	static DVec3 random();
};

DVec3 operator+(const DVec3& v1, double v2);

DVec3 operator+(double v1, const DVec3& v2);

DVec3 operator+(const DVec3& v1, const DVec3& v2);

DVec3 operator-(const DVec3& v1, double v2);

DVec3 operator-(double v1, const DVec3& v2);

DVec3 operator-(const DVec3& v1, const DVec3& v2);

DVec3 operator*(const DVec3& v1, double v2);

DVec3 operator*(double v1, const DVec3& v2);

DVec3 operator*(const DVec3& v1, const DVec3& v2);

DVec3 operator/(const DVec3& v1, double v2);

DVec3 operator/(double v1, const DVec3& v2);

DVec3 operator/(const DVec3& v1, const DVec3& v2);

}
