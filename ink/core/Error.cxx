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

#include "Error.h"

namespace Ink {

std::string Error::get() {
	if (message.empty()) return message;
	return message + '\n';
}

void Error::set(const std::string& m) {
	message = m;
	if (callback) std::invoke(callback, message);
}

void Error::set(const std::string& l, const std::string& m) {
	message = l + " Error: " + m;
	if (callback) std::invoke(callback, message);
}

void Error::clear() {
	message.clear();
}

void Error::set_callback(const ErrorCallback& f) {
	callback = f;
}

std::string Error::message;

Error::ErrorCallback Error::callback;

}
