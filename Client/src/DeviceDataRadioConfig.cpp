#include "DeviceDataStore.hpp"
#include "DataBase.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

int DeviceRadioConfig::getRadioNumber()
{
	int l_radioNumber = 0;
#ifdef CROSS_BUILD
	l_radioNumber = get_radio_number();
#else

#endif
	return l_radioNumber;
}

std::tuple<std::string, int, std::string, int> DeviceRadioConfig::getRadioStatus(int p_index)
{
	char l_radioName[16] = { 0 };
	int l_radioEnable = 0;
	char l_radioPower[16] = { 0 };
	int l_radioChannel = 0;
#ifdef CROSS_BUILD
	get_radio_status(p_index, l_radioName, &l_radioEnable, l_radioPower, &l_radioChannel);
#else

#endif
	return std::make_tuple<std::string, int, std::string, int>(
			l_radioName, l_radioEnable, l_radioPower, l_radioChannel);
}



