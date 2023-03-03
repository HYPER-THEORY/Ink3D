/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "ColorUtils.h"

namespace Ink {

Vec3 ColorUtils::to_rgb(unsigned int c) {
	float r = (c / 0x10000) / 255.f;
	float g = (c / 0x00100 % 0x100) / 255.f;
	float b = (c / 0x00001 % 0x100) / 255.f;
	return {r, g, b};
}

Vec4 ColorUtils::to_rgba(unsigned int c) {
	float r = (c / 0x1000000) / 255.f;
	float g = (c / 0x0010000 % 0x100) / 255.f;
	float b = (c / 0x0000100 % 0x100) / 255.f;
	float a = (c / 0x0000001 % 0x100) / 255.f;
	return {r, g, b, a};
}

unsigned int ColorUtils::to_hex(const Vec3& c) {
	unsigned int r = roundf(c.x * 0xff) * 0x10000;
	unsigned int g = roundf(c.y * 0xff) * 0x00100;
	unsigned int b = roundf(c.z * 0xff) * 0x00001;
	return r + g + b;
}

unsigned int ColorUtils::to_hex(const Vec4& c) {
	unsigned int r = roundf(c.x * 0xff) * 0x1000000;
	unsigned int g = roundf(c.y * 0xff) * 0x0010000;
	unsigned int b = roundf(c.z * 0xff) * 0x0000100;
	unsigned int a = roundf(c.w * 0xff) * 0x0000001;
	return r + g + b + a;
}

}
