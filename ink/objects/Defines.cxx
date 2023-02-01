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

#include "Defines.h"

namespace Ink {

std::string Defines::get() const {
	return defines;
}

void Defines::set(const Defines& d) {
	defines += d.get();
}

void Defines::set(const std::string& n) {
	defines += "#define " + n + "\n";
}

void Defines::set(const std::string& n, const std::string& v) {
	defines += "#define " + n + " " + v + "\n";
}

void Defines::set_i(const std::string& n, int v) {
	defines += "#define " + n + " " + std::to_string(v) + "\n";
}

void Defines::set_l(const std::string& n, long v) {
	defines += "#define " + n + " " + std::to_string(v) + "\n";
}

void Defines::set_ll(const std::string& n, long long v) {
	defines += "#define " + n + " " + std::to_string(v) + "\n";
}

void Defines::set_if(const std::string& n, bool f) {
	if (f) defines += "#define " + n + "\n";
}

}
