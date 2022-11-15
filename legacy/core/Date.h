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

#include <string>

namespace Ink::Legacy {

struct Date {
	int year = 0;				/**< the full year */
	int month = 0;				/**< the month (0 for January) */
	int day = 0;				/**< the day of month, from 1 to 31 */
	int hours = 0;				/**< the hours from 0 to 23 */
	int minutes = 0;			/**< the minutes from 0 to 59 */
	int seconds = 0;			/**< the seconds from 0 to 59 */
	int milliseconds = 0;		/**< the seconds from 0 to 999 */
};

/**
 * Returns the milliseconds since midnight January 1, 1970.
 */
long long get_time();

/**
 * Returns the date according to local time.
 */
Date get_date();

/**
 * Returns the date according to universal time.
 */
Date get_utc_date();

/**
 * Returns a string for the date in YYYY-MM-DD HH:MM:SS format.
 */
std::string format_date(const Date& d);

}
