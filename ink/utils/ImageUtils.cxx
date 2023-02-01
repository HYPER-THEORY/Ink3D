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

#include "ImageUtils.h"

namespace Ink {

float ImageUtils::nearest_sample(const Image& i, int c, float u, float v) {
	int x = roundf(u * (i.width - 1));
	x = x < 0 ? 0 : x;
	x = x >= i.width ? i.width - 1 : x;
	int y = roundf(v * (i.height - 1));
	y = y < 0 ? 0 : y;
	y = y >= i.height ? i.height - 1 : y;
	if (i.bytes == 1) {
		return i.data[(x + y * i.width) * i.channel + c] / 255.f;
	} else {
		auto* data = reinterpret_cast<const float*>(i.data.data());
		return data[(x + y * i.width) * i.channel + c] / 255.f;
	}
}

float ImageUtils::nearest_sample(const Image& i, int c, const Vec2& uv) {
	return nearest_sample(i, c, uv.x, uv.y);
}

float ImageUtils::linear_sample(const Image& i, int c, float u, float v) {
	int x_0 = u * (i.width - 1);
	x_0 = x_0 < 0 ? 0 : x_0;
	x_0 = x_0 >= i.width ? i.width - 1 : x_0;
	int x_1 = x_0 + 1;
	x_1 = x_1 >= i.width ? i.width - 1 : x_1;
	int y_0 = v * (i.height - 1);
	y_0 = y_0 < 0 ? 0 : y_0;
	y_0 = y_0 >= i.height ? i.height - 1 : y_0;
	int y_1 = y_0 + 1;
	y_1 = y_1 >= i.height ? i.height - 1 : y_1;
	if (i.bytes == 1) {
		auto* data = i.data.data();
		float v_0 = data[(x_0 + y_0 * i.width) * i.channel + c];
		float v_1 = data[(x_0 + y_1 * i.width) * i.channel + c];
		float v_2 = data[(x_1 + y_0 * i.width) * i.channel + c];
		float v_3 = data[(x_1 + y_1 * i.width) * i.channel + c];
		return (v_0 * (y_1 - v) + v_1 * (v - y_0)) * (x_1 - u) +
			(v_2 * (y_1 - v) + v_3 * (v - y_0)) * (u - x_0);
	} else {
		auto* data = reinterpret_cast<const float*>(i.data.data());
		float v_0 = data[(x_0 + y_0 * i.width) * i.channel + c];
		float v_1 = data[(x_0 + y_1 * i.width) * i.channel + c];
		float v_2 = data[(x_1 + y_0 * i.width) * i.channel + c];
		float v_3 = data[(x_1 + y_1 * i.width) * i.channel + c];
		return (v_0 * (y_1 - v) + v_1 * (v - y_0)) * (x_1 - u) +
			(v_2 * (y_1 - v) + v_3 * (v - y_0)) * (u - x_0);
	}
}

float ImageUtils::linear_sample(const Image& i, int c, const Vec2& uv) {
	return linear_sample(i, c, uv.x, uv.y);
}

}
