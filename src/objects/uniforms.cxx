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

#include "uniforms.h"

uniforms::uniforms(std::initializer_list<std::pair<std::string, const void*> > l) {
	for (auto& [n, v] : l) {
		set(n, v);
	}
}

const void* uniforms::get(const std::string& n) const {
	return vars.at(n);
}

void uniforms::set(const std::string& n, const void* v) {
	vars.insert_or_assign(n, v);
}

bool uniforms::has(const std::string& n) const {
	return vars.count(n) != 0;
}

std::unordered_map<std::string, const void*>::const_iterator uniforms::begin() const {
	return vars.begin();
}

std::unordered_map<std::string, const void*>::const_iterator uniforms::end() const {
	return vars.end();
}
