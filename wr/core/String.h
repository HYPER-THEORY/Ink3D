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

#include <vector>
#include <string>
#include <sstream>
#include "Math.h"

namespace WR {

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
std::string to_string(const Vec2f& v, int p = 2);

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
std::string to_string(const Vec3f& v, int p = 2);

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
std::string to_string(const Vec4f& v, int p = 2);

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
template <int r, int c>
std::string to_string(const Matf<r, c>& v, int p = 2);

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
std::string to_string(const Vec2d& v, int p = 2);

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
std::string to_string(const Vec3d& v, int p = 2);

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
std::string to_string(const Vec4d& v, int p = 2);

/**
 * Converts a value to a string.
 *
 * \param v value to be converted
 * \param p floating-point decimal precision
 */
template <int r, int c>
std::string to_string(const Matd<r, c>& v, int p = 2);

/* template implementations */

template <int r, int c>
std::string to_string(const Matf<r, c>& v, int p) {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	for (int i = 0; i < r; ++i) {
		stream << "[ ";
		for (int j = 0; j < c; ++j) {
			stream << v[i][j];
			if (j != c - 1) stream << ", ";
		}
		stream << " ]\n";
	}
	return stream.str();
}

template <int r, int c>
std::string to_string(const Matd<r, c>& v, int p) {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	for (int i = 0; i < r; ++i) {
		stream << "[ ";
		for (int j = 0; j < c; ++j) {
			stream << v[i][j];
			if (j != c - 1) stream << ", ";
		}
		stream << " ]\n";
	}
	return stream.str();
}

}
