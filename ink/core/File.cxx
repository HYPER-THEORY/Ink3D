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

#include "File.h"

#include <cstring>
#include <fstream>
#include <limits>

namespace Ink {

std::string File::read(const std::string& p) {
	std::string content;
	std::ifstream stream(p, std::fstream::in);
	if (!stream) {
		Error::set("File: Error reading from file");
	}
	stream.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize length = stream.gcount();
	content.resize(length);
	stream.seekg(0, stream.beg);
	stream.read(content.data(), length);
	stream.close();
	return content;
}

void File::write(const std::string& p, const std::string& c) {
	std::ofstream stream(p, std::fstream::out);
	stream.write(c.data(), c.size());
	if (!stream) {
		Error::set("File: Error writing to file");
	}
	stream.close();
}

void File::write(const std::string& p, const char* c) {
	std::ofstream stream(p, std::fstream::out);
	stream.write(c, std::strlen(c));
	if (!stream) {
		Error::set("File: Error writing to file");
	}
	stream.close();
}

void File::append(const std::string& p, const std::string& c) {
	std::ofstream stream(p, std::fstream::out | std::fstream::app);
	stream.write(c.data(), c.size());
	if (!stream) {
		Error::set("File: Error appending to file");
	}
	stream.close();
}

void File::append(const std::string& p, const char* c) {
	std::ofstream stream(p, std::fstream::out | std::fstream::app);
	stream.write(c, std::strlen(c));
	if (!stream) {
		Error::set("File: Error appending to file");
	}
	stream.close();
}

}
