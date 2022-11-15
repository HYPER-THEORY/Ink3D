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

#include "Date.h"

#include <chrono>

namespace Ink::Legacy {

long long get_time() {
	using namespace std::chrono;
	auto now = system_clock::now().time_since_epoch();
	return duration_cast<milliseconds>(now).count();
}

Date get_date() {
	long long time_ms = get_time();
	time_t time_s = time_ms / 1000;
	tm local = *localtime(&time_s);
	return {
		local.tm_year + 1900, local.tm_mon,
		local.tm_mday, local.tm_hour,
		local.tm_min, local.tm_sec,
		static_cast<int>(time_ms % 1000),
	};
}

Date get_utc_date() {
	long long time_ms = get_time();
	time_t time_s = time_ms / 1000;
	tm local = *gmtime(&time_s);
	return {
		local.tm_year + 1900, local.tm_mon,
		local.tm_mday, local.tm_hour,
		local.tm_min, local.tm_sec,
		static_cast<int>(time_ms % 1000),
	};
}

std::string format_date(const Date& d) {
	int date[6] = {
		d.year, d.month + 1, d.day,
		d.hours, d.minutes, d.seconds,
	};
	const char* delimiter = " -- ::";
	std::string formatted = std::to_string(date[0]);
	for (int i = 1; i < 6; ++i) {
		formatted.append(1, delimiter[i]);
		if (date[i] < 10) formatted.append(1, '0');
		formatted.append(std::to_string(date[i]));
	}
	return formatted;
}

}
