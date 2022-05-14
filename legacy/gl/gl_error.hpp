/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GL_ERROR_HPP
#define GL_ERROR_HPP

#include "gl_stdinc.hpp"

namespace gl {

enum {
	NO_ERROR,			/* No errors */
	ERROR_NO_MEMORY,	/* Out of memory */
	ERROR_READ,			/* Error reading from file */
	ERROR_WRITE,		/* Error writing to file */
	ERROR_SEEK,			/* Error seeking in file */
	ERROR_UNSOPPORTED,	/* That operation is not supported */
	ERROR_UNKNOWN,		/* Unknown error */
};

class error {
public:
	/**
	 * \param c error code
	 * \param m error message
	 */
	static void set(int c, std::string m = "") {
		error_code = c;
		if (m.empty()) {
			if (c == ERROR_NO_MEMORY) {
				message = "Out of memory";
			} else if (c == ERROR_READ) {
				message = "Error reading from file";
			} else if (c == ERROR_WRITE) {
				message = "Error writing to file";
			} else if (c == ERROR_SEEK) {
				message = "Error seeking in file";
			} else if (c == ERROR_UNSOPPORTED) {
				message = "That operation is not supported";
			} else {
				message = "Unknown error";
			}
		} else {
			message = m;
		}
	}
	
	static int get_code() {
		return error_code;
	}
	
	static std::string get_message() {
		if (error_code == NO_ERROR) return "";
		return message + '\n';
	}
	
	static void clear() {
		error_code = NO_ERROR;
	}
	
private:
	static int error_code;
	static std::string message;
};

inline int error::error_code = NO_ERROR;
inline std::string error::message;

}

#endif
