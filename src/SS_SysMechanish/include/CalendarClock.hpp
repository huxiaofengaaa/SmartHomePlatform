/*
 * CalendarClock.hpp
 *
 *  Created on: 2018��7��13��
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
