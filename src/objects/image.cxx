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

#include "image.h"

void image::slice(image& i, int x, int y, int w, int h) {
	i.width = w;
	i.height = h;
	i.bytes = bytes;
	i.data.resize(w * h * bytes);
	x = std::min(x, width - w);
	y = std::min(y, height - h);
	for (int r = 0; r < h; ++r) {
		std::copy_n(data.data() + x * bytes + (r + y) * width * bytes, w * bytes, i.data.data() + r * w * bytes);
	}
}

void image::flip_vertical() {
	int median = height / 2;
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < median; ++y) {
			for (int b = 0; b < bytes; ++b) {
				std::swap(data[(x + y * width) * bytes + b], data[(x + (height - y - 1) * width) * bytes + b]);
			}
		}
	}
}

void image::flip_horizontal() {
	int median = width / 2;
	for (int x = 0; x < median; ++x) {
		for (int y = 0; y < height; ++y) {
			for (int b = 0; b < bytes; ++b) {
				std::swap(data[(x + y * width) * bytes + b], data[(width - x - 1 + y * width) * bytes + b]);
			}
		}
	}
}

void image::swap_rb() {
	if (bytes < 3) return;
	for (int p = width * height; p --> 0;) {
		std::swap(data[p * bytes], data[p * bytes + 2]);
	}
}
