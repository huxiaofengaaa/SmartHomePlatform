/**************************************************************************
 * FileName    : CalendarClock.cpp
 * Author      : huxiaofeng
 * Create Data : 2018-07-29
 * Revision    :
 * Description : a calendar and clock object used to obtain time-related data;
 * CopyRight   :
 * OtherInfo   : Instruction for use:
 *               1) use CalendarClock class to create an new object,
 *                  auto l_listener = CalendarClock();
 *               2) call interface to obtain information
 * ModifyLog   :
 ***************************************************************************/
#include "CalendarClock.hpp"

std::string CalendarClock::getLocalTimeString()
{
	std::time_t l_now = std::time(nullptr);
	return std::ctime(&l_now);
}

std::tuple<int,int,int,int,int,int> CalendarClock::getLocalTimeGroup()
{
	std::time_t l_now = std::time(nullptr);
	std::tm* l_nowTm = std::localtime(&l_now);
	return std::make_tuple(
			l_nowTm->tm_year + 1990, l_nowTm->tm_mon , l_nowTm->tm_mday ,
			l_nowTm->tm_hour , l_nowTm->tm_min, l_nowTm->tm_sec
	);
}

long CalendarClock::getTimeStamp()
{
	return std::time(nullptr);
}

std::string CalendarClock::to_humanStringTime(long p_timestamp)
{
	std::time_t l_result = p_timestamp;
	return std::ctime(&l_result);
}
