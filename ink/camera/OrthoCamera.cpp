/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

#include "OrthoCamera.h"

namespace ink {

OrthoCamera::OrthoCamera(float le, float ri, float lo, float up, float n, float f) {
	set(le, ri, lo, up, n, f);
}

void OrthoCamera::set(float le, float ri, float lo, float uh, float n, float f) {
	left = le;
	right = ri;
	lower = lo;
	upper = uh;
	far = f;
	near = n;
	projection = {
		        2 / (ri - le),                     0,                     0, (ri + le) / (le - ri),
		                    0,         2 / (uh - lo),                     0, (uh + lo) / (lo - uh),
		                    0,                     0,           2 / (n - f),     (f + n) / (n - f),
		                    0,                     0,                     0,                     1,
	};
}

}
