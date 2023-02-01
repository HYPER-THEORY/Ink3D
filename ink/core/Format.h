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

#pragma once

#include <string>

namespace Ink {

class Format {
public:
	/**
	 * Formats a format-string with arguments. Formatting rules are similar with
	 * std::formatter in C++20.
	 *
	 * \param s format-string
	 * \param a arguments
	 */
	template <typename... Types>
	static std::string format(const char* s, Types... a);
	
private:
	template <typename Ptr, typename Type>
	static void format_args(Ptr p, Type v);
	
	template <typename Ptr>
	static void format_args(Ptr p, char v);
	
	template <typename Ptr>
	static void format_args(Ptr p, wchar_t v);
	
	template <typename Ptr>
	static void format_args(Ptr p, const char* v);
	
	template <typename Ptr>
	static void format_args(Ptr p, const std::string& v);
	
	template <typename Ptr, typename Type, typename... Types>
	static void format_args(Ptr p, Type v, Types... a);
};

template <typename... Types>
std::string Format::format(const char* s, Types... a) {
	std::string args[sizeof...(a)];
	format_args(args, a...);
	std::string formatted;
	bool specifier = false;
	int arg_id = 0;
	int user_arg_id = -1;
	for (int i = 0; s[i] != '\0'; ++i) {
		const char& c = s[i];
		char l = i > 0 ? s[i - 1] : '\0';
		if (c == '{' || c == '}') {
			specifier = !specifier;
		}
		if (c == '{' && c == l) {
			formatted.append(1, '{');
			continue;
		}
		if (c == '}' && c == l) {
			formatted.append(1, '}');
			continue;
		}
		if (c != '}' && !specifier) {
			formatted.append(1, c);
		}
		if (c != '{' && specifier) {
			if (c < '0' || c > '9') continue;
			if (user_arg_id == -1) user_arg_id = 0;
			user_arg_id = user_arg_id * 10 + c - '0';
		}
		if (c != '}' || specifier) {
			continue;
		}
		if (user_arg_id == -1) {
			formatted.append(args[arg_id++]);
		} else {
			formatted.append(args[user_arg_id]);
			user_arg_id = -1;
		}
	}
	return formatted;
}

template <typename Ptr, typename Type>
void Format::format_args(Ptr p, Type v) {
	*p = std::to_string(v);
}

template <typename Ptr>
void Format::format_args(Ptr p, char v) {
	*p = std::string(1, v);
}

template <typename Ptr>
void Format::format_args(Ptr p, wchar_t v) {
	*p = std::string(1, v);
}

template <typename Ptr>
void Format::format_args(Ptr p, const char* v) {
	*p = v;
}

template <typename Ptr>
void Format::format_args(Ptr p, const std::string& v) {
	*p = v;
}

template <typename Ptr, typename Type, typename... Types>
void Format::format_args(Ptr p, Type v, Types... a) {
	format_args(p, v);
	format_args(p + 1, a...);
}

}
