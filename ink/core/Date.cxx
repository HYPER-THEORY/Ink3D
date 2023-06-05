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

#include "Date.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"

#include <chrono>

namespace Ink {

Date::Date(int year, int month, int day, int h, int m, int s, int ms) :
year(year), month(month), day(day), hours(h), minutes(m), seconds(s), milliseconds(ms) {}

std::string Date::format() const {
	return fmt::format("{}-{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}",
					   year, month + 1, day, hours, minutes, seconds);
}

long long Date::get_time() {
	auto now = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

Date Date::get_local(long long t) {
	if (t == -1) t = get_time();
	int tm_ms = t % 1000;
	time_t time_s = t / 1000;
	std::tm local = *std::localtime(&time_s);
	return Date(local.tm_year + 1900, local.tm_mon, local.tm_mday,
				local.tm_hour, local.tm_min, local.tm_sec, tm_ms);
}

Date Date::get_utc(long long t) {
	if (t == -1) t = get_time();
	int tm_ms = t % 1000;
	time_t time_s = t / 1000;
	std::tm local = *std::gmtime(&time_s);
	return Date(local.tm_year + 1900, local.tm_mon, local.tm_mday,
				local.tm_hour, local.tm_min, local.tm_sec, tm_ms);
}

}
