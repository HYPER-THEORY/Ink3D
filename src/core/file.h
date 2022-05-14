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

#include <fstream>

#include "error.h"

/**
 * Read a file into a string of content.
 *
 * \param s the URL to the file
 * \param c string of content
 */
int read_file(const char* s, std::string& c);

/**
 * Read a file into a string of content.
 *
 * \param s the URL to the file
 * \param c string of content
 */
int read_file(const std::string& s, std::string& c);

/**
 * Write the content into a local file.
 *
 * \param s the URL to the file
 * \param c string of content
 */
int write_file(const char* s, const std::string& c);

/**
 * Write the content into a local file.
 *
 * \param s the URL to the file
 * \param c string of content
 */
int write_file(const std::string& s, const std::string& c);
