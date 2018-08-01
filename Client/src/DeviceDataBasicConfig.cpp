#include "DeviceDataStore.hpp"
#include "DataBase.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

int DeviceBasicConfig::getLEDOnOff()
{
	int l_ledStatus = 0;
#ifdef CROSS_BUILD
	l_ledStatus = get_led_status();
#else

#endif
	return l_ledStatus;
}

std::tuple<int, int, int> DeviceBasicConfig::getRoamingStatus()
{
	int l_romaingEnable = 0;
	int l_lowRssi24G = 0;
	int l_lowRssi5G = 0;
#ifdef CROSS_BUILD
	get_roaming_status(&l_romaingEnable, &l_lowRssi24G, &l_lowRssi5G);
#else

#endif
	return std::make_tuple<int, int, int>(l_romaingEnable, l_lowRssi24G, l_lowRssi5G);
}

std::tuple<int, int, std::string> DeviceBasicConfig::getMacFilterStatus()
{
	int l_macFilterEnable = 0;
	int l_macFilterPolicy = 0;
	char l_macFilterEntry[4096] = { 0 };
#ifdef CROSS_BUILD
	get_macfilter_status(&l_macFilterEnable,
			&l_macFilterPolicy, l_macFilterEntry, sizeof(l_macFilterEntry));
#else

#endif
	return std::make_tuple<int, int, std::string>(
			l_macFilterEnable, l_macFilterPolicy, std::string(l_macFilterEntry));
}




