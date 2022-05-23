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

/* rendering side */
enum {
	FRONT_SIDE,
	BACK_SIDE,
	DOUBLE_SIDE,
};

/* image base format */
enum {
	IMAGE_R,
	IMAGE_RG,
	IMAGE_RGB,
	IMAGE_RGBA,
	IMAGE_D,
	IMAGE_DS,
};

/* image sized format */
enum {
	IMAGE_NONE,
	IMAGE_8,
	IMAGE_16,
	IMAGE_16F,
	IMAGE_32F,
	IMAGE_8I,
	IMAGE_16I,
	IMAGE_32I,
	IMAGE_8UI,
	IMAGE_16UI,
	IMAGE_32UI,
};

/* image data type */
enum {
	IMAGE_UBYTE,
	IMAGE_BYTE,
	IMAGE_USHORT,
	IMAGE_SHORT,
	IMAGE_UINT,
	IMAGE_INT,
	IMAGE_HALF_FLOAT,
	IMAGE_FLOAT,
	IMAGE_UINT_24_8,
};

/* comparison function */
enum {
	FUNC_NEVER,
	FUNC_LESS,
	FUNC_EQUAL,
	FUNC_LEQUAL,
	FUNC_GREATER,
	FUNC_NOTEQUAL,
	FUNC_GEQUAL,
	FUNC_ALWAYS,
};

/* stencil operation */
enum {
	STENCIL_ZERO,
	STENCIL_KEEP,
	STENCIL_REPLACE,
	STENCIL_INCR,
	STENCIL_DECR,
	STENCIL_INCR_WRAP,
	STENCIL_DECR_WRAP,
	STENCIL_INVERT,
};
