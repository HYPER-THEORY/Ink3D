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

#include "Format.h"

namespace Ink::Legacy {

std::string Format::str_format(const char* s, ...) {
	va_list args;
	va_start(args, s);
	std::string formatted;
	bool sign = false;
	for (int i = 0; s[i] != '\0'; ++i) {
		char c = s[i];
		if (!sign) {
			if (c != '%') formatted.append(1, c);
			sign = c == '%';
			continue;
		}
		if (c == '%') {
			formatted.append(1, '%');
		} else if (c == 'c') {
			char arg = va_arg(args, int);
			formatted.append(1, arg);
		} else if (c == 's') {
			const char* arg = va_arg(args, char*);
			formatted.append(arg);
		} else if (c == 'd') {
			int arg = va_arg(args, int);
			formatted.append(std::to_string(arg));
		} else if (c == 'f') {
			double arg = va_arg(args, double);
			formatted.append(std::to_string(arg));
		}
		sign = false;
	}
	va_end(args);
	return formatted;
}

}
