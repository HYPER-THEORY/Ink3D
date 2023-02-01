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

#include "PerspCamera.h"

namespace Ink {

PerspCamera::PerspCamera(float fov, float a, float n, float f) {
	set(fov, a, n, f);
}

void PerspCamera::set(float fov, float a, float n, float f) {
	fov_y = fov;
	aspect = a;
	near = n;
	far = f;
	projection = {
		1 / tanf(fov / 2) / a, 0                    , 0                    , 0                    ,
		0                    , 1 / tanf(fov / 2)    , 0                    , 0                    ,
		0                    , 0                    , (n + f) / (n - f)    , 2 * f * n / (n - f)  ,
		0                    , 0                    , -1                   , 0                    ,
	};
}

}
