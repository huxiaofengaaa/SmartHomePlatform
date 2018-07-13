/*
 * CalendarClock.hpp
 *
 *  Created on: 2018Äê7ÔÂ13ÈÕ
 *      Author: Administrator
 */
#pragma once
#include <ctime>
#include <tuple>
#include <string>

class CalendarClock
{
public:
	std::string getLocalTimeString();
	std::tuple<int,int,int,int,int,int> getLocalTimeGroup();
	long getTimeStamp();
};
