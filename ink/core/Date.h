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

namespace Ink {

class Date {
public:
	int year = 0;                /**< the full year, start from 1900 */
	int month = 0;               /**< the month, 0 for January */
	int day = 0;                 /**< the day of month, range from 1 to 31 */
	int hours = 0;               /**< the hours, range from 0 to 23 */
	int minutes = 0;             /**< the minutes, range from 0 to 59 */
	int seconds = 0;             /**< the seconds, range from 0 to 59 */
	int milliseconds = 0;        /**< the milliseconds, range from 0 to 999 */
	
	/**
	 * Creates a new Date.
	 */
	explicit Date() = default;
	
	/**
	 * Creates a new Date and initializes it with year, month, day, hour,
	 * minute, secoud, millisecond.
	 *
	 * \param year the full year, start from 1900
	 * \param month the month, 0 for January
	 * \param day the day of month, range from 1 to 31
	 * \param h the hours, range from 0 to 23
	 * \param m the minutes, range from 0 to 59
	 * \param s the seconds, range from 0 to 59
	 * \param ms the milliseconds, range from 0 to 999
	 */
	explicit Date(int year, int month, int day, int h, int m, int s, int ms);
	
	/**
	 * Returns a string represents the date in YYYY-MM-DD HH:MM:SS format.
	 */
	std::string format() const;
	
	/**
	 * Returns the specified number of milliseconds since January 1, 1970,
	 * 00:00:00.
	 */
	static long long get_time();
	
	/**
	 * Returns the date that is initialized according to local time.
	 */
	static Date get_local();
	
	/**
	 * Returns the date that is initialized according to coordinated universal
	 * time (UTC) time.
	 */
	static Date get_utc();
};

}
