/**
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

#include <functional>
#include <string>

namespace Ink {

using ErrorCallback = std::function<void(const std::string&)>;

class Error {
public:
	/**
	 * Returns the specific string describing the error.
	 */
	static std::string get();
	
	/**
	 * Sets the string describing the error. calling this function will replace
	 * the previous error message.
	 *
	 * \param e error message
	 */
	static void set(const std::string& e);
	
	/**
	 * Clears the current error message.
	 */
	static void clear();
	
	/**
	 * Sets the callback that will be triggered when a new error occurred.
	 *
	 * \param f error callback function
	 */
	static void set_callback(const ErrorCallback& f);
	
private:
	static std::string message;
	
	static ErrorCallback callback;
};

}
