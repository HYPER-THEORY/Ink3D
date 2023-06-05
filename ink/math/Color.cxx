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

#include "Color.h"

#include "Matrix.h"

#include <cmath>

namespace Ink {

const Mat3 RGB_TO_XYZ = {
	0.4124564, 0.3575761, 0.1804375,
	0.2126729, 0.7151522, 0.0721750,
	0.0193339, 0.1191920, 0.9503041
};

const Mat3 XYZ_TO_RGB = {
	 3.2404542, -1.5371385, -0.4985314,
	-0.9692660,  1.8760108,  0.0415560,
	 0.0556434, -0.2040259,  1.0572252
};

const Vec3 HCY = {0.299, 0.587, 0.114};

Vec3 Color::rgb_to_srgb(const Vec3& c) {
	return {rgb_to_srgb(c.x), rgb_to_srgb(c.y), rgb_to_srgb(c.z)};
}

Vec3 Color::srgb_to_rgb(const Vec3& c) {
	return {srgb_to_rgb(c.x), srgb_to_rgb(c.y), srgb_to_rgb(c.z)};
}

Vec3 Color::rgb_to_xyz(const Vec3& c) {
	return RGB_TO_XYZ * c;
}

Vec3 Color::xyz_to_rgb(const Vec3& c) {
	return XYZ_TO_RGB * c;
}

Vec3 Color::rgb_to_hsv(const Vec3& c) {
	Vec3 hcv = rgb_to_hcv(c);
	return {hcv.x, hcv.y / hcv.z, hcv.z};
}

Vec3 Color::hsv_to_rgb(const Vec3& c) {
	return ((hue_to_rgb(c.x) - 1.f) * c.y + 1.f) * c.z;
}

Vec3 Color::rgb_to_hsl(const Vec3& c) {
	Vec3 hcv = rgb_to_hcv(c);
	float lum = hcv.z - hcv.y * 0.5f;
	float sat = hcv.y / (1.f - fabsf(lum * 2.f - 1.f));
	return {hcv.x, sat, lum};
}

Vec3 Color::hsl_to_rgb(const Vec3& c) {
	return (hue_to_rgb(c.x) - 0.5f) * (1.f - fabsf(2.f * c.z - 1.f)) * c.y + c.z;
}

Vec3 Color::rgb_to_hcy(const Vec3& c) {
	Vec3 hcv = rgb_to_hcv(c);
	float y = HCY.dot(c);
	float z = HCY.dot(hue_to_rgb(hcv.x));
	hcv.y *= y < z ? z / y : (1.f - z) / (1.f - y);
	return {hcv.x, hcv.y, y};
}

Vec3 Color::hcy_to_rgb(const Vec3& c) {
	Vec3 rgb = hue_to_rgb(c.x);
	float d = HCY.dot(rgb);
	float y = c.y * (c.z < d ? c.z / d : d < 1.f ? (1.f - c.z) / (1.f - d) : 1.f);
	return (rgb - d) * y + c.z;
}

float Color::saturate(float v) {
	return v < 0 ? 0 : v > 1 ? 1 : v;
}

float Color::rgb_to_srgb(float v) {
	return v <= 0.0031308f ? v * 12.92f : powf(v, 1.f / 2.4f) * 1.055f - 0.055f;
}

float Color::srgb_to_rgb(float v) {
	return v <= 0.04045f ? v / 12.92f : powf((v + 0.055f) / 1.055f, 2.4f);
}

Vec3 Color::hue_to_rgb(float h) {
	float r = fabsf(h * 6.f - 3.f) - 1.f;
	float g = 2.f - fabsf(h * 6.f - 2.f);
	float b = 2.f - fabsf(h * 6.f - 4.f);
	return {saturate(r), saturate(g), saturate(b)};
}

Vec3 Color::rgb_to_hcv(const Vec3& c) {
	Vec4 p = c.y < c.z ? Vec4(c.z, c.y, -1.f, 2.f / 3.f) : Vec4(c.y, c.z, 0.f, -1.f / 3.f);
	Vec4 q = c.x < p.x ? Vec4(p.x, p.y, p.w, c.x) : Vec4(c.x, p.y, p.z, p.x);
	float chr = q.x - fminf(q.w, q.y);
	float hue = fabsf((q.w - q.y) / (6.f * chr) + q.z);
	return {hue, chr, q.x};
}

}
