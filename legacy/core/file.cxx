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

#include "file.h"

int read_file(const char* s, std::string& c) {
	std::ifstream stream(s, std::fstream::in);
	if (!stream) return set_error("[File Error] Error reading from file");
	stream.seekg(0, stream.end);
	size_t length = stream.tellg();
	c.resize(length);
	stream.seekg(0, stream.beg);
	stream.read(c.data(), length);
	stream.close();
	return 0;
}

int read_file(const std::string& s, std::string& c) {
	std::ifstream stream(s, std::fstream::in);
	if (!stream) return set_error("[File Error] Error reading from file");
	stream.seekg(0, stream.end);
	size_t length = stream.tellg();
	c.resize(length);
	stream.seekg(0, stream.beg);
	stream.read(c.data(), length);
	stream.close();
	return 0;
}

int write_file(const char* s, const std::string& c) {
	std::ofstream stream(s, std::fstream::out);
	stream.write(c.data(), c.size());
	stream.close();
	if (!stream) return set_error("[File Error] Error writing to file");
	return 0;
}

int write_file(const std::string& s, const std::string& c) {
	std::ofstream stream(s, std::fstream::out);
	stream.write(c.data(), c.size());
	stream.close();
	if (!stream) return set_error("[File Error] Error writing to file");
	return 0;
}
